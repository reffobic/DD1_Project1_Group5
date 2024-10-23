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


class logicGate
{
private:
    string name;
    int delay;
    vector<bool> inputs;
    vector<string> input_names;
    bool output;
    string output_name;
    string gate_type;
    
public:
    logicGate();
    logicGate(string n, int d, vector<string> inp, string outp, string type);
    int getDelay();
    string getOutName();
    bool evaluate(bool &out);
    
};
#endif /* LogicGate_h */
