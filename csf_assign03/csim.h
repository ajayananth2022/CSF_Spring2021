 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#ifndef __CSIM_H__
#define __CSIM_H__

#include <stdio.h>
#include <set>
#include <map>

bool checkPowerTwo(int num);

std::string hexToBinary(std::string hex_string);

struct Block {
    unsigned tag;
    bool dirty;
    int load_ts; //for fifo
    int access_ts; //for LRU
};

class Simulator {
    private:
        int associativity;
        int num_offset;
        int num_index;
        int num_tag;

        int load_hits;
        int load_misses;
        int store_hits;
        int store_miss;

        std::string write_miss;
        std::string write_hit;
        std::string replace_strategy;
        std::map<int, std::set<Block>> cache;

    public:
        Simulator(char *argv[]); //constructor
        void print_summary(); //print number of loads, stores, etc
        void load(std::string address);
        void store(std::string address);

};



#endif // __CSIM_H__
