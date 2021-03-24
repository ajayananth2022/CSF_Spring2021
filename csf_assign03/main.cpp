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
    string write_miss = argv[4];
    string write_hit = argv[5];
    string replace = argv[6];
    if (atoi(argv[1]) <= 0 || !checkPowerTwo(atoi(argv[1]))) {
	    cout << "Invalid number of sets in the cache!" << endl;
	    return 1;
    }
    if (atoi(argv[2]) <= 0 || !checkPowerTwo(atoi(argv[2]))) {
	    cout << "Invalid number of blocks in the cache!" << endl;
        return 1;
    }
    if (atoi(argv[3]) < 4 || !checkPowerTwo(atoi(argv[3]))) {
	    cout << "Invalid size of blocks in the cache!" << endl;
	    return 1;
    }
    if (write_miss != "write-allocate" && write_miss != "no-write-allocate") {
        cout << "Invalid write-miss strategy!" << endl;
	    return 1;
    }
    if (write_hit != "write-through" && write_hit != "write-back") {
        cout << "Invalid write-hit strategy!" << endl;
	    return 1;
    }
    if (replace != "lru" && replace != "fifo") {
        cout << "Invalid replace strategy!" << endl;
	    return 1;
    }
    if (write_miss == "no-write-allocate" && write_hit == "write-back") {
        cout << "Invalid write-miss and write-hit stratgy combination!" << endl;
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


