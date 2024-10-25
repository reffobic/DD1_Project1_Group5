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
    string verilogFile = "Circuit.v";
    string stimFile = "Circuit.stim";

    ofstream outputFile;
    outputFile.open("output.sim");

    if (!outputFile) {
        return 1;
    }
    else {
        Module module1 = parseVerilogFile(verilogFile);
        queue<event> eventQueue = parseStimFile(stimFile, module1.inputs, module1.wires);

        for (const auto& output : module1.outputs) {
            outputFile << "0," << output.name << "," << output.val << '\n';
        }

        while (!eventQueue.empty()) {
            event current = eventQueue.front();
            eventQueue.pop();
            time += current.delay;

            for (int i = 0; i < current.affectedVarInd.size(); i++) {
                int ind = current.affectedVarInd[i];
                if (current.varType[i] == "input") {
                    if (ind < 0 || ind >= module1.inputs.size()) continue;
                    module1.inputs[ind].val = current.newVals[i];
                } else if (current.varType[i] == "wire") {
                    if (ind < 0 || ind >= module1.wires.size()) continue;
                    module1.wires[ind].val = current.newVals[i];
                }
            }

            for (auto& gate : module1.gates) {
                string outType;
                int outIndex = gate.getOut(outType);

                if (outType == "output" && outIndex < module1.outputs.size()) {
                    bool prevVal = module1.outputs[outIndex].val;
                    gate.evaluate(module1.outputs[outIndex].val, module1.inputs, module1.wires);
                    if (module1.outputs[outIndex].val != prevVal) {
                        outputFile << to_string(time) << ',' << module1.outputs[outIndex].name << ',' << to_string(module1.outputs[outIndex].val) << '\n';
                    }
                } else if (outType == "wire" && outIndex < module1.wires.size()) {
                    bool prevVal = module1.wires[outIndex].val;
                    gate.evaluate(module1.wires[outIndex].val, module1.inputs, module1.wires);
                }
            }
        }
    }
    cout << "Process completed, output file is generated." << endl;
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
    regex delay_pattern(R"(#(\d+))");
    regex assignment_pattern(R"(([a-zA-Z][a-zA-Z0-9_]*)=(0|1))");

    if (file.is_open()) {
        while (getline(file, line)) {
            smatch delay_match;
            if (!regex_search(line, delay_match, delay_pattern)) continue;

            int delay = stoi(delay_match[1]);
            event currentEvent;
            currentEvent.delay = delay;
            currentEvent.affectedVarInd.clear();
            currentEvent.newVals.clear();
            currentEvent.varType.clear();

            auto assignments = sregex_iterator(line.begin(), line.end(), assignment_pattern);
            for (auto it = assignments; it != sregex_iterator(); ++it) {
                smatch match = *it;
                string variable = match[1];
                int value = stoi(match[2]);

                int ind = findIndex(variable, inputs);
                if (ind != -1) {
                    currentEvent.affectedVarInd.push_back(ind);
                    currentEvent.varType.push_back("input");
                    currentEvent.newVals.push_back(value == 1);
                }
            }

            if (!currentEvent.affectedVarInd.empty()) {
                events.push(currentEvent);
            }
        }
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
                if (outInd == -1) {
                    outInd = findIndex(outName, module1.wires);
                    outType = "wire";
                }

                if (outInd == -1) continue;

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
                    }
                }

                logicGate gate(match[3], stoi(match[2]), inp, outInd, match[1], outType, inpTypes);
                module1.gates.push_back(gate);
            }
        }
    }
    file.close();

    return module1;
}
