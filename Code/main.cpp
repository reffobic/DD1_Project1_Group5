//
//  main.cpp
//  DD1_Project1
//
//  Created by Mohamed El-Refai on 14/10/2024.
//

#include <iostream>
#include "LogicGate.hpp"
#include "Circuit.hpp"
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>


using namespace std;

struct iovar
{
    string name;
    bool value;
};

struct Module{
    vector<string> inputs;
    vector<logicGate> gates;
    string output;
    bool outVal;
    vector<wire> wires;
};

int findWire(const string & wireName, vector<wire> wires);

queue<logicGate> verilogFileParser(string &filename, Module &m1); //reads a .v file and parses it to a simulated circuit

void stimFileParser(string);

struct Event{
    int time_inc;
    string outName;
    vector<string> inName;
    vector<bool> inValue;
};

int main()
{
    Module m1;
    string file1 = "/Users/refobic/Documents/DD1/Project1/DD1_Project1/DD1_Project1/Circuit4.v";
    vector<queue<logicGate>> circuit;
    circuit[0] = verilogFileParser(file1, m1);
    int queue_count = 0;
    
    int t = 0;
    
    ifstream file;
    vector<string> lines;
    
    string file2 = "/Users/refobic/Documents/DD1/Project1/DD1_Project1/DD1_Project1/Circuit4.stim";
    
    file.open(file2);
    
    if(!file.is_open()) {
        cout << "Failed to open file " << file2 << endl;
        return 0;
    }
    
    ofstream simFile;
    
    simFile.open("Output.sim");
    
    if (!simFile) {
            cout << "Error: Could not create or open the sim file." << endl;
        }
    
    
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
        stringstream ss(line);
        string word;
        
        Event temp;
        
        while(ss >> word)
        {
            if(word[0] == '#')
            {
                temp.time_inc = stoi(word.substr(1,word.length()));
                ss >> word;
            }
            
            int i = 0;
            while(ss >> word)
            {
                if((word[0] >= 65 && word[0] <= 90) || (word[0] >=97 && word[0] <= 122))
                {
                    temp.inName[i] = word;
                    ss >> word;
                    ss >> word;
                    temp.inValue[i] = stoi(word);
                }
                i++;
            }
            i=0;
        }
        int x = queue_count%2;
        
        while(!circuit[x].empty())
        {
            logicGate top = circuit[x].front();
            t += top.getDelay();
            int ind = findWire(top.getOutName(),m1.wires);
            
            if(ind != -1)
            {
                bool tempbool = false;
                top.evaluate(tempbool);
                m1.wires[ind].setValue(tempbool);
            }
            else
            {
                top.evaluate(m1.outVal);
                simFile << '#' << t << '\t' << m1.output << '\t' << m1.outVal << '\n';
            }
            
            circuit[x].pop();
            circuit[(x+1)%2].push(top);
        }
    }
    
    file.close();
    simFile.close();
}

queue<logicGate> verilogFileParser(string &filename, Module &M1) {
    
    ifstream file;
    vector<string> lines;
    
    file.open(filename);
    
    queue<logicGate> eventsim;
    
    if(!file.is_open()) {
        cout << "Failed to open file " << filename << endl;
        return eventsim;
    }
    
    Module m1;
    string line;
    while (getline(file, line)) {
        lines.push_back(line);
        stringstream ss(line);
        string word;
        
        while (ss >> word) {
            if (word == "input") {
                ss >> word;
                m1.inputs.push_back(word);
                
            } else if (word == "output") {
                ss >> word;
                m1.output = word;
            } else if (word == "wire") {
                ss >> word;
                wire t(word, false, 0);
                m1.wires.push_back(t);
            } else if (word == "and" || word == "or" || word == "not" || word == "nand" || word == "nor" || word == "xor" || word == "xnor") {
                string type = word;
                ss >> word;
                int i=2;
                while (i < word.length() && word[i+1]!=')')
                {
                    i++;
                }
                int delay = stoi(word.substr(2,i));
                ss >> word;
                string name = word;
                ss >> word;
                string out = word.substr(1,1);
                ss >> word;
                vector<string> input_name(2);
                input_name[0] = word.substr(0,0);
                ss >> word;
                input_name[1] = word.substr(0,0);
                logicGate gate(name, delay, input_name, out, type);
                eventsim.push(gate);
            }
        }
        
    }
    
    
    
    file.close();
    return eventsim;
}


int findWire(const string & wireName, vector<wire> wires){
    for (int i = 0; i < wires.size(); i++){
        if(wireName == wires[i].getName()){
            return i;
        }
        }
            cout << "Wire not found";
            return -1;
    
}
