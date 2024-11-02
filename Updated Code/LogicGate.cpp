#include "LogicGate.hpp"


// Constructor sets the values for most of the gate's attributes
logicGate::logicGate(string n, int d, vector<int> inp, int outp, string type, string outputType, vector<string> inTypes) {
    name = n;
    delay = d;
    outputInd = outp;
    gate_type = type;
    outType = outputType;

    for (int i = 0; i < inTypes.size(); i++) {
        ioVar temp;
        temp.val = false;
        temp.type = inTypes[i];
        temp.index = inp[i];
        inputs.push_back(temp);
    }
}

bool logicGate::evaluate(bool& out, vector<ioVar> inputVec, vector<ioVar> wireVec) { // evaluate function to check the output of the gate
    updateInputs(inputVec, wireVec); // updates the inputs to reflect any change of values in the main

    if (gate_type == "and") { // If the gate is an and gate, it checks if any of the inputs are false. If 1 is false, it stores the output as false
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = false;
                break;
            }
        }
    } else if (gate_type == "or") { // If the gate is an or gate, it checks if any of the inputs are true. If so, it stores the output as true
        out = false;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = true;
                break;
            }
        }
    } else if (gate_type == "not") { // if the gate is a not gate, it takes the first input (since there can only be 1 input it is assumed to be the first) and returns its inversion
        out = !inputs[0].val;
    } else if (gate_type == "nand") { // If the gate is a nand gate, it checks for at least 1 0, if it is found it stores the output as true.
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = true;
                break;
            }
        }
        out = false;
    } else if (gate_type == "nor") { // If the gate is a nand gate, it checks for at least 1 1, if so it stores the output as false
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = false;
                break;
            }
        }
    } else if (gate_type == "xor") { // If the gate is an xor gate, it checks if there is an odd number of 1s. If so, it stores the output as true
        int trueCount = 0;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                trueCount++;
            }
        }
        out = (trueCount % 2 == 1);
    } else if (gate_type == "xnor") { // If the gate is an xnor gate, it checks if there is an even number of 1s. If so, it stores the output as true.
        int trueCount = 0;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                trueCount++;
            }
        }
        out = (trueCount % 2 == 0);
    } else {
        return false;
    }

    return out;
}

int logicGate::getDelay() { // returns the delay
    return delay;
}

int logicGate::getOut(string& type) { // stores the type in the parameter and returns the index of the gate's output variable
    type = outType;
    return outputInd;
}

void logicGate::updateInputs(vector<ioVar> inputVars, vector<ioVar> wires) { // updates the inputs of the gate to reflect any change in values in the main.
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i].type == "input") {
            inputs[i].val = inputVars[inputs[i].index].val;
        } else if (inputs[i].type == "wire") {
            inputs[i].val = wires[inputs[i].index].val;
        }
    }
}
