#include "LogicGate.hpp"

logicGate::logicGate() {
    name = "";
}

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

bool logicGate::evaluate(bool& out, vector<ioVar> inputVec, vector<ioVar> wireVec) {
    updateInputs(inputVec, wireVec);

    if (gate_type == "and") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = false;
                break;
            }
        }
    } else if (gate_type == "or") {
        out = false;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = true;
                break;
            }
        }
    } else if (gate_type == "not") {
        out = !inputs[0].val;
    } else if (gate_type == "nand") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (!inputs[i].val) {
                out = true;
                return out;
            }
        }
        out = false;
    } else if (gate_type == "nor") {
        out = true;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                out = false;
                break;
            }
        }
    } else if (gate_type == "xor") {
        int trueCount = 0;
        for (int i = 0; i < inputs.size(); i++) {
            if (inputs[i].val) {
                trueCount++;
            }
        }
        out = (trueCount % 2 == 1);
    } else if (gate_type == "xnor") {
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

int logicGate::getDelay() {
    return delay;
}

int logicGate::getOut(string& type) {
    type = outType;
    return outputInd;
}

void logicGate::updateInputs(vector<ioVar> inputVars, vector<ioVar> wires) {
    for (int i = 0; i < inputs.size(); i++) {
        if (inputs[i].type == "input") {
            inputs[i].val = inputVars[inputs[i].index].val;
        } else if (inputs[i].type == "wire") {
            inputs[i].val = wires[inputs[i].index].val;
        }
    }
}
