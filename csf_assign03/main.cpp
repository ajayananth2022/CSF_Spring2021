 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#include <iostream>
#include <string>
#include "csim.h"

using std::cout;
using std::endl;
using std::string; 
using std::cin;

int main(int argc, char **argv) {
    if (argc != 7) {
	    cout << "Invalid number of arguments!" << endl;
	    return 1;
    }
    Simulator sim(argv);
    string mem_access;
    while (getline(cin, mem_access)) {
        string address = hexToBinary(mem_access.substr(4, 8));

        if (mem_access[0] == 's') {
            sim.store(address);
        } else if (mem_access[0] == 'l') {
            sim.load(address);
        }
    }
    sim.printSummary();
    
    return 0; 
}


