#include "LogicGate.hpp"

logicGate::logicGate() {
    name = "";
}

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

// evaluate function to check the output of the gate
bool logicGate::evaluate(bool& out, vector<ioVar> inputVec, vector<ioVar> wireVec) {
    // updates the inputs to reflect any change of values in the main
    updateInputs(inputVec, wireVec);

    // If the gate is an and gate, it checks if any of the inputs are false. If 1 is false, it stores the output as false
    if (gate_type == "and") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = false;
                break;
            }
        }
    }
    // If the gate is an or gate, it checks if any of the inputs are true. If so, it stores the output as true
    else if (gate_type == "or") {
        out = false;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = true;
                break;
            }
        }
    }
    // if the gate is a not gate, it takes the first input (since there can only be 1 input it is assumed to be the first) and returns its inversion
    else if (gate_type == "not") {
        out = !inputs[0].val;
    
    }
    // If the gate is a nand gate, it checks for at least 1 0, if it is found it stores the output as true.
    else if (gate_type == "nand") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = true;
                return out;
            }
        }
        out = false;
    }
    // If the gate is a nor gate, it checks for at least 1 1, if so it stores the output as false
    else if (gate_type == "nor") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = false;
                break;
            }
        }
    }
    // If the gate is an xor gate, it checks if there is an odd number of 1s. If so, it stores the output as true
    else if (gate_type == "xor") {
        int trueCount = 0;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                trueCount++;
            }
        }
        out = (trueCount % 2 == 1);
    }
    // If the gate is an xnor gate, it checks if there is an even number of 1s. If so, it stores the output as true.
    else if (gate_type == "xnor") {
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

    return true;
}

// returns the delay
int logicGate::getDelay() {
    return delay;
}

// stores the type in the parameter and returns the index of the gate's output variable
int logicGate::getOut(string& type) {
    type = outType;
    return outputInd;
}

// updates the inputs of the gate to reflect any change in values in the main.
void logicGate::updateInputs(vector<ioVar> inputVars, vector<ioVar> wires) {
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i].type == "input") {
            inputs[i].val = inputVars[inputs[i].index].val;
        } else if (inputs[i].type == "wire") {
            inputs[i].val = wires[inputs[i].index].val;
        }
    }
}
