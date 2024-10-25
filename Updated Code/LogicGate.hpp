//
//  LogicSimulator.hpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 23/10/2024.
//

#ifndef LogicGate_hpp
#define LogicGate_hpp

#include <stdio.h>
#include <iostream>
#include <vector>
using namespace std;

struct ioVar {
    string name;
    bool val;
    int index;
    vector<int> affectedGateInd;
    string type;
};

class logicGate
{
private:
    string name;
    int delay;
    vector<ioVar> inputs;
    bool output;
    int outputInd;
    string outType;
    string gate_type;
    void updateInputs(vector<ioVar> inputVars, vector<ioVar> wireVars);

public:
    logicGate();
    logicGate(string n, int d, vector<int> inp, int outp, string type, string outType, vector<string> inTypes);
    int getDelay();
    int getOut(string& type);
    bool evaluate(bool &out, vector<ioVar> inputVec, vector<ioVar>wireVec);
};
#endif /* LogicGate_h */