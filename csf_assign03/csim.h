 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#ifndef __CSIM_H__
#define __CSIM_H__

#include <stdio.h>
#include <set>
#include <map>

bool checkPowerTwo(int num);

std::string hexToBinary(std::string hex_string);

class Block {
    public:
        std::string tag;
        bool dirty; //for it the cache is different from the main memory
        int load_ts; //for fifo
        int access_ts; //for LRU
        Block(std::string t, bool d) {
            tag = t;
            dirty = d;
            load_ts = 0;
            access_ts = 0;
        }

class Simulator {
    private:
        int associativity;
        int num_offset;
        int num_index;
        int num_tag;

        int load_hits;
        int load_misses;
        int store_hits;
        int store_misses;

        std::string write_miss;
        std::string write_hit;
        std::string replace_strategy;

        std::map<std::string, std::set<Block>> cache; //key is index, value is a set of blocks

    public:
        Simulator(char *argv[]); //constructor
        void printSummary(); //print number of loads, stores, etc
        void load(std::string address);
        void store(std::string address);
        //Block* checkHit(std::string address);

};



#endif // __CSIM_H__
