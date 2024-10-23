//
//  Circuit.hpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 23/10/2024.
//

#ifndef Circuit_hpp
#define Circuit_hpp

#include "LogicGate.hpp"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

class wire{
    
private:
    string name;
    bool value;
    int lastChanged;
    
public:
    wire(string name, bool value, int lastChanged);
    int getLastChanged();
    void setLastChanged(int lastChange);
    bool getValue();
    void setValue(int valu);
    string getName();
    void setName(string n);
};

class logicSimulator {

private:
    vector<wire> wires;
    vector<logicGate> gates;
public:
    void addWire(wire w);
    void addGate(logicGate);
    
    wire findWire(const string & wireName);
    void connectCircuit();
    void simulateCircuit();
};



#endif /* Circuit_hpp */
