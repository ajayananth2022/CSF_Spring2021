 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu

#include <iostream>
#include "csim.h"

using std::cout;
using std::endl;
using std::string; 

bool checkPowerTwo(int num) {
    if ((num & (num - 1)) == 0) {
        return true; 
    }
    return false; 
}

Simulator::Simulator(int argc, char *argv[]) {
    if (argc != 7) {
	    cout << "Invalid number of arguments!" << endl;
	    return NULL;
    }
    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int size_blocks = atoi(argv[3]);
    string write_miss = argv[4];
    string write_hit = argv[5];
    string replace_strategy = argv[6];

    if (num_sets <= 0 || !checkPowerTwo(num_sets)) {
	    cout << "Invalid number of sets in the cache!" << endl;
	    exit(1);
    }
    if (num_blocks <= 0 || !checkPowerTwo(num_blocks)) {
	    cout << "Invalid number of blocks in the cache!" << endl;
        exit(1);
    }
    if (size_blocks < 4 || !checkPowerTwo(size_blocks)) {
	    cout << "Invalid size of blocks in the cache!" << endl;
	    exit(1);
    }
    if (write_miss != "write-allocate" && write_miss != "no-write-allocate") {
	    exit(1);
    }
    if (write_hit != "lru" && write_hit != "fifo") {
	    exit(1);
    }

    num_sets = num_sets; 
    num_blocks = num_blocks; 
    size_blocks = size_blocks; 
    write_miss = write_miss; 
    write_hit = write_hit; 
    replace_strategy = replace_strategy; 
}