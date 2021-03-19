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
			case 'a': 
            case 'A':
                retString.append ("1010"); 
                break;
			case 'b': 
            case 'B':
                retString.append ("1011"); 
                break;
			case 'c': 
            case 'C':
                retString.append ("1100"); 
                break;
			case 'd': 
            case 'D':
                retString.append ("1101"); 
                break;
			case 'e': 
            case 'E':
                retString.append ("1110"); 
                break;
			case 'f': 
            case 'F':
                retString.append ("1111"); 
                break;
            default: cout << "invalid address" << endl; break;
	    }
	}
    return retString;
}

Simulator::Simulator(char *argv[]) {
    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int size_blocks = atoi(argv[3]);
    string write_miss_strategy = argv[4];
    string write_hit_strategy = argv[5];
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
    if (write_miss_strategy != "write-allocate" && write_miss_strategy != "no-write-allocate") {
        cout << "Invalid write-miss strategy!" << endl;
	    exit(1);
    }
    if (write_hit_strategy != "write-through" && write_hit_strategy != "write-back") {
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
    write_miss = write_miss_strategy; 
    write_hit = write_hit_strategy; 
    replace = replace_strategy; 
    load_hits = 0;
    load_misses = 0;
    store_hits = 0;
    store_misses = 0;
    cycle_main_mem = 0;
    map<string, vector<Block> > cache;
}

void Simulator::printSummary() {
    int total_loads = load_hits + load_misses;
    int total_stores = store_hits + store_misses;
    int total_cycles = total_loads + total_stores + cycle_main_mem;

    cout << "Total loads: " << total_loads << endl;
    cout << "Total store: " << total_stores << endl;
    cout << "Load hits: " << load_hits << endl; 
    cout << "Load misses: " << load_misses << endl; 
    cout << "Store hits: " << store_hits << endl; 
    cout << "Store misses: " << store_misses << endl; 
    cout << "Total cycles: " << total_cycles << endl;
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
                return;
            } 
        }

        //at this point, the mem block to be loaded is NOT in set

        //if set is full, evict a block
        if ((int)cache.at(index).size() == associativity) {
            bool evictBlockDirty = false; 
            if (replace == "lru") {
                int access = INT_MAX;
                vector<Block>::iterator least_used;
                for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
                    if (it->access_ts < access) { //find the block with smallest access time
                        access = it->access_ts;
                        least_used = it;
                    }
                }
<<<<<<< HEAD
=======
                if (least_used->dirty == true) {
                    evictBlockDirty = true; 
                }
>>>>>>> bfa9c314f8a1772bf1061d922ffb89387689ceda
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
                if (first_in->dirty == true) {
                    evictBlockDirty = true; 
                }
                cache.at(index).erase(first_in);
            }
            
            //write back to main memory takes 100 * (block size/ 4) cycles , ONLY if dirty??
            if (write_hit == "write-back" && evictBlockDirty == true) cycle_main_mem += 100 * ((2^num_offset)/4) + 1; 
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
    load_misses++;
    cycle_main_mem += 100 * ((2^num_offset)/4); //bring in memory takes 100 cycles
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

                bool evictBlockDirty = false; 

                if (replace == "lru") {
                    int access = INT_MAX;
                    vector<Block>::iterator least_used;
                    for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
                        if (it->access_ts < access) {
                            access = it->access_ts;
                            least_used = it;
                        }
                    }

                    if (least_used->dirty == true) {
                        evictBlockDirty = true; 
                    }
                    cache.at(index).erase(least_used);

                } else { //fifo
                    int load = -1;
                    vector<Block>::iterator first_in;
                    for (it = cache.at(index).begin(); it != cache.at(index).end(); it++) {
                        if (it->load_ts > load) {
                            load = it->access_ts;
                            first_in = it;
                        }
                    }

                    if (first_in->dirty == true) {
                        evictBlockDirty = true; 
                    }
                cache.at(index).erase(first_in);
                }
                
                //only if DIRTY?
                if (write_hit == "write-back" && evictBlockDirty == true) cycle_main_mem += 100 * ((2^num_offset)/4); //write memory takes 100 cycles
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
            cycle_main_mem += 100; //writes ONLY modified memory ( not ENTIRE block) directly to memory 
            return; 
        }
    }
    cycle_main_mem += 100 * ((2^num_offset)/4);
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
