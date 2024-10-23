//
//  LogicSimulator.cpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 23/10/2024.
//

#include "LogicGate.hpp"

logicGate :: logicGate(string n, int d, vector<string> inp, string outp, string type)
{
    name = n;
    delay = d;
    input_names = inp;
    output_name = outp;
    gate_type = type;
}

bool logicGate :: evaluate(bool &out)
{
    if(gate_type == "and")
    {
        for(int i=0; i<inputs.size(); i++)
        {
            if(inputs[i] == 0)
            {
                out = 0;
                return out;
            }
        }
        out = 1;
        return out;
    }
    else if ( gate_type == "or")
    {
        for(int i=0; i<inputs.size(); i++)
        {
            if(inputs[i] == 1)
            {
                out = 1;
                return out;
            }
        }
        out = 0;
        return out;
    }
    else if (gate_type == "not")
    {
        if(inputs[0])
            return 0;
        out = 1;
        return out;
    }
    else if (gate_type == "nand")
    {
        for(int i=0; i<inputs.size(); i++)
        {
            if(inputs[i] == 0)
            {
                out = 1;
                return out;
            }
        }
        out = 0;
        return out;
    }
    else if (gate_type == "nor")
    {
        for(int i=0; i<inputs.size(); i++)
        {
            if(inputs[i] == 1)
            {
                out = 0;
                return out;
            }
        }
        out = 1;
        return out;
    }
    else if (gate_type == "xor")
    {
        int s = 0;
        for(int i=0; i<inputs.size(); i++)
        {
            s += inputs[i];
        }
        if(s%2 == 1)
        {
            out = 1;
            return out;
        }
        out = 0;
        return out;
    }
    else if (gate_type == "xnor")
    {
        int s = 0;
        for(int i=0; i<inputs.size(); i++)
        {
            s += inputs[i];
        }
        if(s%2 == 1)
        {
            out = 0;
            return out;
        }
        out = 1;
        return out;
    } else {
        out = NULL;
        return out;
    }
}

int logicGate:: getDelay()
{
    return delay;
}

string logicGate :: getOutName()
{
    return output_name;
}
