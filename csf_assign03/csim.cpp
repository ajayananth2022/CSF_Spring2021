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

    string retString = "";
	for (int i = 0; i < hex_string.length (); ++i) {
		switch (hex_string [i]){
			case '0': retString.append ("0000"); break;
			case '1': retString.append ("0001"); break;
			case '2': retString.append ("0010"); break;
			case '3': retString.append ("0011"); break;
			case '4': retString.append ("0100"); break;
			case '5': retString.append ("0101"); break;
			case '6': retString.append ("0110"); break;
			case '7': retString.append ("0111"); break;
			case '8': retString.append ("1000"); break;
			case '9': retString.append ("1001"); break;
			case 'a': retString.append ("1010"); break;
			case 'b': retString.append ("1011"); break;
			case 'c': retString.append ("1100"); break;
			case 'd': retString.append ("1101"); break;
			case 'e': retString.append ("1110"); break;
			case 'f': retString.append ("1111"); break;
	    }
	}
    return retString;
}

Simulator::Simulator(char *argv[]) {
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
    map<string, set<Block> > cache;
}

void Simulator::print_summary() {
    //TO-DO

    cout<<"Load hits: " + load_hits<<endl; 
    cout<<"Load misses: " + load_misses<<endl; 
}

void Simulator::load(string address) {

    //TO-DO

    string tag = address.substr(0, num_tag - 1);
    string index = address.substr(num_tag - 1, num_tag + num_index - 1);

    bool cacheHit = false; 

    //search for index (key in map)
    if (cache.count(index) == 1) {
        set<Block>:: iterator it; 
        //search for particular tag in index
        for (it = cache[index].begin(); it!=cache[index].end(); it++) {
            if (it->tag==tag) {
                //we have a load hit
                //increment load_hits and update load_ts & access_ts
                cacheHit = true; 
                load_hits++; 
            } 
        }
    }
    
    //load miss
    if (!cacheHit) {
        //if index is not present, or index is present but tag is different
        //we have a load miss
        //increment load_misses 
        load_misses++; 
    }
}

void Simulator::store(string address) {
    //TO-DO

    //search for index (key in map)
    //if index is present, tag is the same, we have a write hit
    //increment store_hits

    //if index is not present, or index is present but tag is different
    //we have a write miss
    //increment store_misses

    //write hit:
    //write through: "write data" to memory (dirty bit always false), update access_ts
    //write back: "write data" to cache, update access_ts

    //write miss:
    //write-allocate: load into cache, update dirty bit, update access_ts, replace if needed
    //no-write-allocate: "write data" to memory (dirty bit always false)

    //replace:
    //LRU: evict the least recently accessed
    //FIFO: evict the first in (oldest)

}