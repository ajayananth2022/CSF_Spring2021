 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu

#include <iostream>
#include <math.h>
#include "csim.h"
#include <vector>
#include <map>
#include <climits>

using std::cout;
using std::endl;
using std::string; 
using std::map;
using std::vector;

bool checkPowerTwo(int num) {
    if ((num & (num - 1)) == 0) {
        return true; 
    }
    return false; 
}

string hexToBinary(string hex_string) {
    string retString = "";
	for (int i = 0; i < (int)hex_string.length(); ++i) {
		switch (hex_string[i]){
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
            case 'A': retString.append ("1010"); break;
			case 'b': retString.append ("1011"); break;
            case 'B': retString.append ("1011"); break;
			case 'c': retString.append ("1100"); break;
            case 'C': retString.append ("1100"); break;
			case 'd': retString.append ("1101"); break;
            case 'D': retString.append ("1101"); break;
			case 'e': retString.append ("1110"); break;
            case 'E': retString.append ("1110"); break;
			case 'f': retString.append ("1111"); break;
            case 'F': retString.append ("1111"); break;
            default: cout << "invalid address" << endl; break;
	    }
	}
    return retString;
}

Simulator::Simulator(char *argv[]) {
    associativity = atoi(argv[2]); 
    num_offset = log2(atoi(argv[3]));
    num_index = log2(atoi(argv[1]));
    num_tag = 32 - num_offset - num_index;
    write_miss = argv[4];
    write_hit = argv[5]; 
    replace = argv[6]; 
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_misses = 0;
    cycle_main_mem = 0;
    map<string, vector<Block>> cache;
}

void Simulator::printSummary() {
    int total_loads = load_hits + load_misses;
    int total_stores = store_hits + store_misses;
    int total_cycles = cycle_main_mem;

    cout << "Total loads: " << total_loads << endl;
    cout << "Total stores: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl; 
    cout << "Load misses: " << load_misses << endl; 
    cout << "Store hits: " << store_hits << endl; 
    cout << "Store misses: " << store_misses << endl; 
    cout << "Total cycles: " << total_cycles << endl;
}

bool Simulator::evict(string strategy, string index) {
    bool evictBlockDirty = false;
    vector<Block>::iterator it; 
    if (strategy == "lru") {
        int access = INT_MAX;
        vector<Block>::iterator least_used;
        for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            if (it->access_ts < access) { //find the block with smallest access time
                access = it->access_ts;
                least_used = it;
            }
        }
        if (least_used->dirty == true) evictBlockDirty = true; 
        cache.at(index).erase(least_used); //remove least accessed
    } else { //fifo
        int load = -1;
        vector<Block>::iterator first_in;
        for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            if (it->load_ts > load) { //find the block with the biggest load time
                load = it->access_ts;
                first_in = it;
            }
        }
        if (first_in->dirty == true) evictBlockDirty = true; 
        cache.at(index).erase(first_in);
    }
    return evictBlockDirty;
}

void Simulator::load(string address) {
    string tag = address.substr(0, num_tag); //num_tag is number of tag bits
    string index = address.substr(num_tag, num_index);
    //search for index (key in map)
    if (cache.count(index) == 1) { //if map element is found
        vector<Block>::iterator it; 
        //search for particular tag in index
        for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            if (it->tag == tag) { //load hit is found
                load_hits++;
                it->access_ts++; //update access time
                cycle_main_mem++; //1 cycle: load from cache to CPU
                return;
            } 
        }
        //at this point, the mem block to be loaded is NOT in set

        //if set is full, evict a block
        if ((int)cache.at(index).size() == associativity) {
            bool evictBlockDirty = evict(replace, index);
            //write back to main memory takes 100 * (block size/ 4) cycles , ONLY if dirty
            if (evictBlockDirty == true) cycle_main_mem += 100 * ((1<<num_offset)/4); 
        }

        //if there's no block in the set with the particular tag
        for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            it->load_ts++; //increment load time for all old blocks
        }
        Block new_block = Block(tag, false); //create new block with specific tag
        cache.at(index).push_back(new_block);

    } else { //if there's no set with the particulat index        
        vector<Block> new_set; //create new map element with new block
        Block new_block = Block(tag, false);
        new_set.push_back(new_block);
        cache.insert({index, new_set});
    }
    cycle_main_mem += 100 * ((1<<num_offset)/4) + 1; //bring in memory block and load to CPI (+1)
    load_misses++;
}

void Simulator::store(string address) {
    string tag = address.substr(0, num_tag); //num_tag is number of tag bits
    string index = address.substr(num_tag, num_index);
    //search for index (key in map)
    if (cache.count(index) == 1) {
        vector<Block>::iterator it; 
        //search for particular tag in index
        for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
            if (it->tag == tag) { //load hit is found
                store_hits++;
                it->access_ts++;
                cycle_main_mem++; //1 cycle: write to cache from CPU 
                if (write_hit == "write-back") {
                    it->dirty = true; //block and main memory different
                } else { //write-through
                    cycle_main_mem += 100; //write ONLY modified mem ref to main memory (100 cycles)
                }
                return;
            } 
        }

        //at this point, block is not in set, so increment store_misses
        store_misses++;
        //write modified memory directly to main mem and we are DONE
        if (write_miss == "no-write-allocate") {
            cycle_main_mem += 100; //writes ONLY modified memory (not ENTIRE block) directly to memory (no bringing into cache)  
            return; 
        } else { //write-allocate
            //first, evict a block 
            if ((int)cache.at(index).size() == associativity) {
                bool evictBlockDirty = evict(replace, index);
                //write back to main memory takes 100 * (block size/ 4) cycles , ONLY if dirty
                if (evictBlockDirty == true) cycle_main_mem += 100 * ((1<<num_offset)/4); 
            }

            for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
                it->load_ts++; //increment load time for all old blocks
            }
            Block new_block = Block(tag, true); //block and main memory different
            cache.at(index).push_back(new_block);       
        }
    } else {//set with requested index does NOT exist
        store_misses++; 
        if (write_miss == "write-allocate") {
            vector<Block> new_set; //creates new map element with new block
            Block new_block = Block(tag, true); //block and main memory different
            new_set.push_back(new_block);
            cache.insert({index, new_set});
        } else { //no-write-allocate
            cycle_main_mem += 100; //writes modified memory directly to memory 
            return; 
        }
    }
    cycle_main_mem += 100 * ((1<<num_offset)/4);
    //if index is present, tag is the same, we have a write hit
    //increment store_hits

    //if index is not present, or index is present but tag is different
    //we have a write miss
    //increment store_misses

    //write hit:
    //write through: "write data" to memory 
    //write back: "write data" to cache, update access_ts

    //write miss:
    //write-allocate: load into cache, update dirty bit, update access_ts, replace if needed
    //no-write-allocate: "write data" to memory (dirty bit always false)

    //replace:
    //LRU: evict the least recently accessed
    //FIFO: evict the first in (oldest)

}
