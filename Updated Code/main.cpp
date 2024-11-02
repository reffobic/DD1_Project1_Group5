#include <iostream>
#include "LogicGate.hpp"
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>
#include <unordered_map>

using namespace std;

//struct representing modules read from Verilog files
//each module has inputs, ouputs, wires, and gates
struct Module {
    vector<ioVar> inputs; // stores all the input variables
    vector<ioVar> outputs; // stores all the output variables
    vector<ioVar> wires; // stores all the wire variables
    vector<logicGate> gates; // stores all the gates
};

//struct representing a change in the circuit, read from .stim file
struct event {
    int delay; // stores the time the event is triggered in the simulation
    vector<int> affectedVarInd; // stores all the variables changed in the simulation
    vector<string> varType; // stores the type of each of the variables changed (input or wire)
    vector<bool> newVals; // stores the new values of the changed variables
};


// Compare class used for the priority queue to create a min heap for event driven simulation
class Compare {
    public:
       bool operator()(event a, event b){
        return a.delay > b.delay;
      }
};


int findIndex(const string& Name, vector<ioVar> vars); // declaration of function that finds index of a variable (used for finding the index of a specific input, output, or wire)
priority_queue<event, vector<event>, Compare> parseStimFile(const string& filename, vector<ioVar> inputs, vector<ioVar> wires); // declaration of function that reads the stim file and generates the event queue
Module parseVerilogFile(const string& filename); // declaration of function that reads the verilog file and generates the modules

void propagateGateChange(logicGate& gate, Module& module, int time,
                         unordered_map<string, bool>& lastKnownStates, ofstream& outputFile,
                         priority_queue<event, vector<event>, Compare>& eventQueue, event& newWireEvent); // declaration of function used to process events in the gates


int main() {
    int time = 0; // initial simulation time starts at 0
    string verilogFile = "/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/Circuit.v"; // string variable containing the path to the verilog file
    string stimFile = "/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/Circuit.stim"; // string variable containing the path to the stim file

    // The output file is created and titled "output.sim"
    ofstream outputFile;
    outputFile.open("/Users/refobic/Documents/DD1/Project1_G5/DD1_Project1_G5/DD1_Project1_G5/output.sim");

    if (!outputFile) { // program is terminated if the file couldn't be found or created
        return 1;
    } else {
        Module module1 = parseVerilogFile(verilogFile); // a module that takes the module read from the verilog file
        priority_queue<event, vector<event>, Compare> eventQueue = parseStimFile(stimFile, module1.inputs, module1.wires); // the queue of events from the stim file to be processed

        // Initialize the output file with the initial states of inputs, wires, and outputs.
        for (const auto& input : module1.inputs) {
            outputFile << "0," << input.name << "," << input.val << '\n';
        }
        for (const auto& output : module1.outputs) {
            outputFile << "0," << output.name << "," << output.val << '\n';
        }
        for (const auto& wire : module1.wires) {
            outputFile << "0," << wire.name << "," << wire.val << '\n';
        }

        // Main event processing loop for all inputs and wires
        unordered_map<string, bool> lastKnownStates; // Track last known states to prevent processing gates when not needed

        while (!eventQueue.empty()) { // Goes through all events read from stim file
            event current = eventQueue.top();
            eventQueue.pop();
            time = current.delay; // updates the time for the next event

            for (int i = 0; i < current.affectedVarInd.size(); i++) { // loops over all the variables changed in the event
                int ind = current.affectedVarInd[i];

                // Processes the event for when the variable changed is an input
                if (current.varType[i] == "input") { 
                    bool previousVal = module1.inputs[ind].val;
                    module1.inputs[ind].val = current.newVals[i];

                    // Trigger gates when one of their inputs changes
                    for (auto& gateIdx : module1.inputs[ind].affectedGateInd) {
                        logicGate& gate = module1.gates[gateIdx];
                        event newWireEvent; // Creates an event which will contain any variables affected if the gate's output changes
                        propagateGateChange(gate, module1, time, lastKnownStates, outputFile, eventQueue, newWireEvent);
                        if (!newWireEvent.affectedVarInd.empty()) { // checks that the event contains something, otherwise it will not be pushed
                            eventQueue.push(newWireEvent);
                        }
                    }

                    // The file is only written to if the input is changed
                    if (module1.inputs[ind].val != previousVal && lastKnownStates[module1.inputs[ind].name] != module1.inputs[ind].val) {
                        lastKnownStates[module1.inputs[ind].name] = module1.inputs[ind].val; // updates the last known state to avoid processing redundant gates
                        outputFile << time << "," << module1.inputs[ind].name << "," << module1.inputs[ind].val << '\n';
                    }
                }

                // The code below is identical to the code above for the input condition, however it handles the wires vector in stead of the inputs vector
                if (current.varType[i] == "wire") {
                    bool previousVal = module1.wires[ind].val;
                    module1.wires[ind].val = current.newVals[i];

                    for (auto& gateIdx : module1.wires[ind].affectedGateInd) {
                        event newWireEvent;
                        logicGate& gate = module1.gates[gateIdx];
                        propagateGateChange(gate, module1, time, lastKnownStates, outputFile, eventQueue, newWireEvent);
                        if (!newWireEvent.affectedVarInd.empty()) {
                            eventQueue.push(newWireEvent);
                        }
                    }

                    if (module1.wires[ind].val != previousVal && lastKnownStates[module1.wires[ind].name] != module1.wires[ind].val) {
                        lastKnownStates[module1.wires[ind].name] = module1.wires[ind].val;
                        outputFile << time << "," << module1.wires[ind].name << "," << module1.wires[ind].val << '\n';
                    }
                }
            }
        }
    }
    cout << "Process completed, output file is generated." << endl; // Output to tell the user when the simulation is complete
    outputFile.close();
    return 0;
}

int findIndex(const string& Name, vector<ioVar> vars) { // function to find the index of a variable using its name. This is to allow the program to handle variables with their index rather than name.
    string trimmedName = Name;
    trimmedName.erase(remove(trimmedName.begin(), trimmedName.end(), ' '), trimmedName.end()); // removes any spaces from the variable name to avoid errors

    for (int i = 0; i < vars.size(); i++) {
        if (trimmedName == vars[i].name) {
            return i;
        }
    }
    return -1;
}


// Function to parse the stim file and generate the event queue
priority_queue<event, vector<event>, Compare> parseStimFile(const string& filename, vector<ioVar> inputs, vector<ioVar> wires) {
    priority_queue<event, vector<event>, Compare> events;
    ifstream file(filename);
    string line;
    regex delay_pattern(R"(#(\d+))"); // regex pattern to identify delays
    regex assignment_pattern(R"(([a-zA-Z][a-zA-Z0-9_]*)=(0|1))"); // regex pattern to identify any variable changes
    int t = 0; // initializes the simulation time as 0
    if (file.is_open()) {
        while (getline(file, line)) {
            smatch delay_match;
            if (!regex_search(line, delay_match, delay_pattern)) continue; // checks whether a delay match has been found. If not, the rest of the loop is neglected until the next match

            t+= stoi(delay_match[1]); // sets the time to the time the first event will take us to.
            event currentEvent; // creates a temporary event to store the read changes
            currentEvent.delay = t; // sets the time the event will be triggered

            auto assignments = sregex_iterator(line.begin(), line.end(), assignment_pattern);
            for (auto it = assignments; it != sregex_iterator(); ++it) { // loops over the rest of the line to identify any input changes
                smatch match = *it;
                string variable = match[1]; // stores the name of the input changed
                int value = stoi(match[2]); // stores the new value of the input changed

                int ind = findIndex(variable, inputs); // finds the index of the input in the inputs vector.
                if (ind != -1) { // Checks if the found index is valid
                    currentEvent.affectedVarInd.push_back(ind); // pushes the index, type, and value of the input to the current event.
                    currentEvent.varType.push_back("input");
                    currentEvent.newVals.push_back(value == 1);
                }
            }

            if (!currentEvent.affectedVarInd.empty()) { // checks if the event is empty. If it is not, it is pushed to the eventQueue
                events.push(currentEvent);
            }
        }
    }
    file.close();
    return events; // the min heap of events is returned
}


// Function to generate a module based on the verilog file.
Module parseVerilogFile(const string& filename) {
    Module module1;
    ifstream file(filename);

    if (file.is_open()) { // Checks if the file is opened
        string line;
        int input_index = 0; // creates a counter for the index of the next input, output, and wire
        int output_index = 0;
        int wire_index = 0;

        regex input_regex(R"(input\s+([a-zA-Z_]\w*(?:\s*,\s*[a-zA-Z_]\w*)*);)"); // regex pattern to identify inputs
        regex output_regex(R"(output\s+([a-zA-Z_]\w*(?:\s*,\s*[a-zA-Z_]\w*)*);)"); // regex pattern to identify outputs
        regex wire_regex(R"(wire\s+([a-zA-Z_]\w*(?:\s*,\s*[a-zA-Z_]\w*)*);)"); // regex pattern to identify wires
        regex gate_regex(R"((\w+)\s*#\((\d+)\)\s*([a-zA-Z_]\w*)\s*\(([^)]+)\);)"); // regex pattern to identify gates

        int gate_index = 0;  // creates a counter for the index of the next gate

        while (getline(file, line)) {
            smatch match;

            // Condition for if an input is found
            if (regex_search(line, match, input_regex)) {
                string inputs = match[1];
                stringstream ss(inputs);
                string input;
                while (getline(ss, input, ',')) {
                    input.erase(remove(input.begin(), input.end(), ' '), input.end()); // removes any spaces from the input's name
                    ioVar temp{ input, false, input_index++ }; // creates a variable containing the input's name, value, and its current index in the module. It initializes it with 0
                    module1.inputs.push_back(temp); // pushes the input to the module
                }
            } else if (regex_search(line, match, output_regex)) { // Condition for if an output is found
                string outputs = match[1];
                stringstream ss(outputs);
                string output;
                while (getline(ss, output, ',')) {
                    output.erase(remove(output.begin(), output.end(), ' '), output.end()); // removes any spaces from the output's name
                    ioVar temp{ output, false, output_index++ }; // Creates a temporary variable containing the output's data and pushes it to the module
                    module1.outputs.push_back(temp);
                }
            } else if (regex_search(line, match, wire_regex)) { // Condition for if a wire is found
                string wires = match[1];
                stringstream ss(wires);
                string wire;
                while (getline(ss, wire, ',')) {
                    wire.erase(remove(wire.begin(), wire.end(), ' '), wire.end()); // removes any spaces from the wire's name
                    ioVar temp{ wire, false, wire_index++ }; // creates a temporary variable containing the wire's data and pushes it to the module.
                    module1.wires.push_back(temp);
                }
            } else if (regex_search(line, match, gate_regex)) { // Condition for if a gate is found
                string inputs = match[4];
                stringstream ss(inputs);
                string input;
                vector<int> inp; // vector to store the inputs of the gate
                string outName; // string to store the name of the gate
                vector<string> inpTypes; // vector to store whether each input is a wire or an input

                getline(ss, input, ',');
                outName = input; // sets the name of the gate's output to the first one of the "inputs" string as the first variable in a gate declaration is the output.
                outName.erase(remove(outName.begin(), outName.end(), ' '), outName.end()); // removes any spaces from the name

                int outInd = findIndex(outName, module1.outputs); // checks if the output exists in the module
                string outType = "output"; // sets its type to output
                if (outInd == -1) { // if the output is not found, it checks the wires
                    outInd = findIndex(outName, module1.wires); // stores the new index and the type as wire
                    outType = "wire";
                }

                if (outInd == -1) continue; // if it is still not found in the wires vector, it is skipped

                while (getline(ss, input, ',')) { // loops over all inputs in the gate
                    input.erase(remove(input.begin(), input.end(), ' '), input.end()); // removes any spaces from each input's name
                    int ind = findIndex(input, module1.inputs); // finds the index in the inputs vector
                    if (ind != -1) { // Condition for if the variable is found in the inputs vector
                        inp.push_back(ind); // adds the input index to the gate's inputs
                        inpTypes.push_back("input"); // pushes the type of the input to the gate
                        module1.inputs[ind].affectedGateInd.push_back(gate_index); // adds the gate's index to the vector in the inputs containing the affected gates
                    } else { // Condition for if the variable is not found in the inputs
                        ind = findIndex(input, module1.wires);
                        if (ind != -1) { // Checks that the variable is a wire
                            inp.push_back(ind); // repeats the other condition but for if the variable is a wire
                            inpTypes.push_back("wire");
                            module1.wires[ind].affectedGateInd.push_back(gate_index);
                        }
                    }
                }

                logicGate gate(match[3], stoi(match[2]), inp, outInd, match[1], outType, inpTypes); // Creates an object of logicGate and pushes it to the module.
                module1.gates.push_back(gate);
                gate_index++;
            }
        }
    }
    file.close();

    return module1;
}


// Function to process the change of inputs to a gate
void propagateGateChange(logicGate& gate, Module& module, int time,
                         unordered_map<string, bool>& lastKnownStates, ofstream& outputFile,
                         priority_queue<event, vector<event>, Compare>& eventQueue, event& newWireEvent) {
    string outType; 
    int outIndex = gate.getOut(outType); // the index and the output type (wire or output) of the gate is fetched from the object

    bool newVal = false;
    gate.evaluate(newVal, module.inputs, module.wires); // the gate is evaluated and its value is stored

    string varName = (outType == "output") ? module.outputs[outIndex].name : module.wires[outIndex].name; // stores the wire or output name of the gate

    // runs only if the output of the gate changes
    if (lastKnownStates[varName] != newVal) { 
        time += gate.getDelay(); // sets the time for a new event to be created from any other affected gates using the delay
        lastKnownStates[varName] = newVal; // updates the last known state of the gate output

        // Update output file only if there's a change from the last known state
        if (outType == "output" && outIndex < module.outputs.size()) {
            module.outputs[outIndex].val = newVal; // updates the value in the module too
            outputFile << time << "," << module.outputs[outIndex].name << "," << newVal << '\n';
        } else if (outType == "wire" && outIndex < module.wires.size()) {
            module.wires[outIndex].val = newVal;
            outputFile << time << "," << module.wires[outIndex].name << "," << newVal << '\n';

            // Add the current wire to the new event if there are any changes to check if these changes affect other gates
            newWireEvent.delay = time;
            newWireEvent.affectedVarInd.push_back(outIndex);
            newWireEvent.varType.push_back("wire");
            newWireEvent.newVals.push_back(newVal);
        }
    }
}
