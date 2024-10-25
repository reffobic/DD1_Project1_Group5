//
//  LogicSimulator.cpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 23/10/2024.
//

#include "LogicGate.hpp"

logicGate::logicGate()
{
    name = "";
}

logicGate::logicGate(string n, int d, vector<int> inp, int outp, string type, string outputType, vector<string> inTypes)
{
    name = n;
    delay = d;
    outputInd = outp;
    gate_type = type;
    outType = outputType;

    for (int i = 0; i < inTypes.size(); i++)
    {
        ioVar temp;
        temp.val = false;
        temp.type = inTypes[i];
        temp.index = inp[i];
        inputs.push_back(temp);
    }
}

bool logicGate::evaluate(bool &out, vector<ioVar> inputVec, vector<ioVar> wireVec)
{
    updateInputs(inputVec, wireVec);
    if (gate_type == "and")
    {
        for (int i = 0; i < inputs.size(); i++)
        {
            if (!inputs[i].val)
            {
                output = false;
                out = false;
                return output;
            }
        }
        output = true;
        out = true;
        return output;
    }
    else if (gate_type == "or")
    {
        for (int i = 0; i < inputs.size(); i++)
        {
            if (inputs[i].val)
            {
                output = true;
                out = true;
                return output;
            }
        }
        output = false;
        out = false;
        return output;
    }
    else if (gate_type == "not")
    {
        if (inputs[0].val)
        {
            output = false;
            out = false;
            return false;
        }
            
        output = true;
        out = true;
        return output;
    }
    else if (gate_type == "nand")
    {
        for (int i = 0; i < inputs.size(); i++)
        {
            if (!inputs[i].val)
            {
                output = true;
                out = true;
                return output;
            }
        }
        output = false;
        out = false;
        return output;
    }
    else if (gate_type == "nor")
    {
        for (int i = 0; i < inputs.size(); i++)
        {
            if (inputs[i].val)
            {
                output = false;
                return output;
            }
        }
        output = true;
        out = true;
        return output;
    }
    else if (gate_type == "xor")
    {
        int s = 0;
        for (int i = 0; i < inputs.size(); i++)
        {
            if (inputs[i].val)
                s++;
        }
        if (s % 2 == 1)
        {
            output = true;
            out = true;
            return output;
        }
        output = false;
        out = false;
        return output;
    }
    else if (gate_type == "xnor")
    {
        int s = 0;
        for (int i = 0; i < inputs.size(); i++)
        {
            if (inputs[i].val)
                s++;
        }
        if (s % 2 == 1)
        {
            output = false;
            out = false;
            return output;
        }
        output = true;
        out = true;
        return output;
    }
    else {
        output = NULL;
        out = NULL;
        return output;
    }
}

int logicGate::getDelay()
{
    return delay;
}

int logicGate::getOut(string& type)
{
    type = outType;
    return outputInd;
}

void logicGate::updateInputs(vector<ioVar> inputVars, vector<ioVar> wires)
{
    for (int i = 0; i < inputs.size(); i++)
    {
        if (inputs[i].type == "input")
            inputs[i].val = inputVars[inputs[i].index].val;
        if (inputs[i].type == "wire")
            inputs[i].val = wires[inputs[i].index].val;
    }
}