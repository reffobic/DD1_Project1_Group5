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

struct ioVar { // struct for inputs, outputs, or wires
    string name; // stores the name
    bool val; // stores the value
    int index; // stores the index in the vector to avoid having to search for it
    vector<int> affectedGateInd; // stores the index of any gates for which this variable is an input
    string type; // stores the type of variable (wire, input, output)
};

class logicGate // Class for the gates
{
private:
    string name; // stores the name of the gate
    int delay; // stores the propagation delay of this gate
    vector<ioVar> inputs; // stores the input variables of this gate
    bool output; // stores the output value of this gate
    int outputInd; // stores the index of the output in its relevant vector
    string outType; // stores the type of output (wire or output)
    string gate_type; // stores the gate type (and, or, etc.)
    void updateInputs(vector<ioVar> inputVars, vector<ioVar> wireVars); // function used to update the values of the vectors to reflect those in the main

public:
    logicGate(string n, int d, vector<int> inp, int outp, string type, string outType, vector<string> inTypes); // Constructor to initialize attributes
    int getDelay(); // getter returns the propagation delay of the gate
    int getOut(string& type); // getter to return the index of the output and its type by reference
    bool evaluate(bool &out, vector<ioVar> inputVec, vector<ioVar>wireVec); // function to evaluate the gate
};
#endif /* LogicGate_h */
