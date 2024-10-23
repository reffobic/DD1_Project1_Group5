//
//  Circuit.cpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 23/10/2024.
//

#include "Circuit.hpp"

wire :: wire(string name, bool value, int lastChanged) {
    this->name = name;
    this->value = value;
    this->lastChanged = lastChanged;
    
}
int wire :: getLastChanged(){
    return lastChanged; //time
}

void wire :: setLastChanged(int lastChange){
    lastChanged = lastChange; //set the time when it was last changed
}

bool wire :: getValue(){
    return value; //value of wire
}

void wire :: setValue(int valu){
    value = valu; //set the value of the wire
}

string wire :: getName(){
    return name;
}

void logicSimulator :: addWire(wire w){
    wires.push_back(w);
}

void logicSimulator :: addGate(logicGate g){
    gates.push_back(g);
}

wire logicSimulator :: findWire(const string & wireName){
    for (int i = 0; i < wires.size(); i++){
        if(wireName == wires[i].getName()){
            return wires[i];
        }
        }
            wire w2("0", 0,0);
            cout << "Wire not found";
            return w2;
    
}

void wire :: setName(string n)
{
    name = n;
}
