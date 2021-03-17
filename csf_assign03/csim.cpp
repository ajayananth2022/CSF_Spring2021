 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu

#include <iostream>
#include <math.h>
#include "csim.h"
#include <set>
#include <map>

using std::cout;
using std::endl;
using std::string; 
using std::set;
using std::map;

bool checkPowerTwo(int num) {
    if ((num & (num - 1)) == 0) {
        return true; 
    }
    return false; 
}

string hexToBinary(string hex_string) {
    //TO-DO
    return "\0";
}

Simulator::Simulator(int argc, char *argv[]) {
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
        cout << "Invalid write-miss strategy!" << endl;
	    exit(1);
    }
    if (write_hit != "write-through" && write_hit != "write-back") {
        cout << "Invalid write-hit strategy!" << endl;
	    exit(1);
    }

    if (replace_strategy != "lru" && replace_strategy != "fifo") {
        cout << "Invalid replace strategy!" << endl;
	    exit(1);
    }

    associativity = num_blocks; 
    map<int, vector<string>> cache;
    num_offset = log2(size_blocks);
    num_index = log2(num_sets);
    num_tag = 32 - num_offset - num_index;
    write_miss = write_miss; 
    write_hit = write_hit; 
    replace_strategy = replace_strategy; 
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_miss = 0;
    map<int, set<Block>> cache;
}

void Simulator::print_summary() {
    //TO-DO

}

void Simulator::load(string address) {
    //TO-DO

}

void Simulator::store(string address) {
    //TO-DO

}