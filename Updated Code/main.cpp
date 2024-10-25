#include <iostream>
#include "LogicGate.hpp"
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

struct Module {
    vector<ioVar> inputs;
    vector<ioVar> outputs;
    vector<ioVar> wires;
    vector<logicGate> gates;
};

struct event {
    int delay;
    vector<int> affectedVarInd;
    vector<string> varType;
    vector<bool> newVals;
};

int findIndex(const string& Name, vector<ioVar> vars);
queue<event> parseStimFile(const string& filename, vector<ioVar> inputs, vector<ioVar> wires);
Module parseVerilogFile(const string& filename);

int main() {
    int time = 0;
    string verilogFile = "C:/Users/moham/Videos/Uni/Sem 5 - Fall 2024/Digital Design I/WORK/Circuit4.v";
    string stimFile = "C:/Users/moham/Videos/Uni/Sem 5 - Fall 2024/Digital Design I/WORK/Circuit4.stim";

    ofstream outputFile;
    outputFile.open("C:/Users/moham/Videos/Uni/Sem 5 - Fall 2024/Digital Design I/WORK/output.sim");

    if (!outputFile) {
        cout << "Error: Could not create output file.\n";
    }
    else {
        Module module1 = parseVerilogFile(verilogFile);
        queue<event> eventQueue = parseStimFile(stimFile, module1.inputs, module1.wires);

        while (!eventQueue.empty()) {
            event current = eventQueue.front();
            eventQueue.pop();
            time += current.delay;

            int cur = current.affectedVarInd.size();
            for (int i = 0; i < cur; i++) {
                ioVar* currentVar = nullptr;
                int ind = -1;

                ind = current.affectedVarInd[i];
                if (current.varType[i] == "input")
                {
                    if (ind < 0 || ind >= module1.inputs.size())
                    {
                        cout << "Error: Invalid index " << ind << " for module1.inputs." << endl;
                        continue;
                    }
                    module1.inputs[ind].val = current.newVals[i];
                    currentVar = &module1.inputs[ind];
                }
                else if (current.varType[i] == "wire")
                {
                    if (ind < 0 || ind >= module1.wires.size())
                    {
                        cout << "Error: Invalid index " << ind << " for module1.wires." << endl;
                        continue;
                    }
                    module1.wires[ind].val = current.newVals[i];
                    currentVar = &module1.wires[ind];
                }

                if (currentVar != nullptr) {
                    for (int j = 0; j < currentVar->affectedGateInd.size(); j++) {
                        string t = "";
                        int x = currentVar->affectedGateInd[j];
                        int outIndex = module1.gates[x].getOut(t);
                        if (x < 0 || x >= module1.gates.size()) {
                            cout << "Error: Invalid gate index " << j << " in module1.gates.\n";
                            continue;
                        }

                        if (outIndex > -1 && t == "output" && outIndex < module1.outputs.size()) {
                            module1.gates[x].evaluate(module1.outputs[outIndex].val, module1.inputs, module1.wires);
                            time += module1.gates[x].getDelay();
                            outputFile << to_string(time) << ',' << module1.outputs[outIndex].name << ',' << to_string(module1.outputs[outIndex].val) << '\n';
                        }
                        else if (outIndex > -1 && t == "wire" && outIndex < module1.wires.size()) {
                            module1.gates[x].evaluate(module1.wires[outIndex].val, module1.inputs, module1.wires);
                            current.affectedVarInd.push_back(outIndex);
                            current.varType.push_back("wire");
                            current.newVals.push_back(module1.wires[outIndex].val);
                            cur++;
                            time += module1.gates[x].getDelay();
                        }
                        else {
                            cout << "Error: outIndex " << outIndex << " is out of bounds for type " << t << endl;
                            continue;
                        }
                    }
                }
            }
        }
    }
    cout<<"Simulation complete. Open output.sim file.\n";
    outputFile.close();
    return 0;
}

int findIndex(const string& Name, vector<ioVar> vars) {
    for (int i = 0; i < vars.size(); i++) {
        if (Name == vars[i].name) {
            return i;
        }
    }
    return -1;
}

queue<event> parseStimFile(const string& filename, vector<ioVar> inputs, vector<ioVar> wires) {
    queue<event> events;
    ifstream file(filename);
    string line;
    regex pattern(R"(#(\d+)\s+([a-zA-Z][a-zA-Z0-9_]*)=(0|1);)");

    if (file.is_open()) {
        while (getline(file, line)) {
            smatch match;
            bool newEvent = true;
            event currentEvent;

            while (regex_search(line, match, pattern)) {
                int delay = stoi(match[1]);
                string variable = match[2];
                int value = stoi(match[3]);

                if (newEvent) {
                    currentEvent.delay = delay;
                    currentEvent.affectedVarInd.clear();
                    currentEvent.newVals.clear();
                    currentEvent.varType.clear();
                    newEvent = false;
                }

                int ind = findIndex(variable, inputs);
                if (ind != -1) {
                    currentEvent.affectedVarInd.push_back(ind);
                    currentEvent.varType.push_back("input");
                    if (value != 0)
                        currentEvent.newVals.push_back(true);
                    else
                        currentEvent.newVals.push_back(false);
                }
                else {
                    cout << "Error: Could not find variable " << variable << "\n";
                }

                line = line.substr(match.position() + match.length());
            }
            if (!newEvent) {
                events.push(currentEvent);
            }
        }
    }
    else {
        cout << "Error: could not open stim file.\n";
    }
    file.close();
    return events;
}

Module parseVerilogFile(const string& filename) {
    Module module1;
    ifstream file(filename);

    if (file.is_open()) {
        string line;
        int input_index = 0;
        int output_index = 0;
        int wire_index = 0;

        regex input_regex(R"(input\s+([a-zA-Z_]\w*);)");
        regex output_regex(R"(output\s+([a-zA-Z_]\w*);)");
        regex wire_regex(R"(wire\s+([a-zA-Z_]\w*);)");
        regex gate_regex(R"((\w+)\s*#\((\d+)\)\s*([a-zA-Z_]\w*)\s*\(([^)]+)\);)");

        while (getline(file, line)) {
            smatch match;

            if (regex_search(line, match, input_regex)) {
                ioVar temp{ match[1], false, input_index++ };
                module1.inputs.push_back(temp);
            }
            else if (regex_search(line, match, output_regex)) {
                ioVar temp{ match[1], false, output_index++ };
                module1.outputs.push_back(temp);
            }
            else if (regex_search(line, match, wire_regex)) {
                ioVar temp{ match[1], false, wire_index++ };
                module1.wires.push_back(temp);
            }
            else if (regex_search(line, match, gate_regex)) {
                string inputs = match[4];
                stringstream ss(inputs);
                string input;
                vector<int> inp;
                string outName;
                vector<string> inpTypes;

                getline(ss, input, ',');
                outName = input;

                int outInd = findIndex(outName, module1.outputs);
                string outType = "output";
                if (outInd == -1) 
                {
                    outInd = findIndex(outName, module1.wires);
                    outType = "wire";
                }

                if (outInd == -1) {
                    cout << "Error: Could not find output or wire for " << outName << endl;
                    continue;
                }

                while (getline(ss, input, ',')) {
                    int ind = findIndex(input, module1.inputs);
                    if (ind != -1) {
                        inp.push_back(ind);
                        inpTypes.push_back("input");
                        module1.inputs[ind].affectedGateInd.push_back(module1.gates.size());
                    }
                    else {
                        ind = findIndex(input, module1.wires);
                        if (ind != -1) {
                            inp.push_back(ind);
                            inpTypes.push_back("wire");
                            module1.wires[ind].affectedGateInd.push_back(module1.gates.size());
                        }
                        else {
                            cout << "Error: Could not find input or wire for " << input << endl;
                        }
                    }
                }

                logicGate gate(match[3], stoi(match[2]), inp, outInd, match[1], outType, inpTypes);
                module1.gates.push_back(gate);
            }
        }
    }
    else {
        cout << "Error: Could not open verilog file.\n";
    }
    file.close();
    return module1;
}
