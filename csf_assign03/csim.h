 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#ifndef __CSIM_H__
#define __CSIM_H__

#include <stdio.h>
#include <vector>
#include <map>


/*
 * helper function that checks if a number is a power of 2
 * @param num the number to be checked
 * @return true if the number is power of 2, false otherwise
 */
bool checkPowerTwo(int num);


/*
 * helper function that converts a string of hexadecimal numbers to binary
 * @param hex_string the string in hexadecimal to be converted
 * @return a string with binary representation of the hex number
 */
std::string hexToBinary(std::string hex_string);


/*
 * A class to represent a block in cache
 */
class Block {
    public:
        std::string tag; //a string of binary numbers
        bool dirty; //true if the cache is different from main memory
        int load_ts; //for fifo
        int access_ts; //for LRU

        /*
         * Constructor of the Block class
         * @param t the tag of block
         * @param d whether the cache is different from main memory
         */
        Block(std::string t, bool d) {
            tag = t;
            dirty = d;
            load_ts = 0;
            access_ts = 0;
        }
};


/*
 * A class that represents a cache simulator
 * contains basic info about the cache
 * stores the number of load and store hits/misses
 */
class Simulator {
    private:
        int associativity;
        int num_offset; //number of offset bits
        int num_index; //number of index bits
        int num_tag; //number of tag bits

        int load_hits;
        int load_misses;
        int store_hits;
        int store_misses;
        int cycle_main_mem; //total number of cycles

        std::string write_miss; //write-miss strategy
        std::string write_hit; //write-hit strategy
        std::string replace; //replacement strategy

        //key is index, value is a vector of blocks
        std::map<std::string, std::vector<Block>> cache; 

    public:
        /*
         * Constructor of the Simulator class from command line args
         * @param argv contains the command line args that specifies
         * the dimension and strategies used in the cache
         */
        Simulator(char *argv[]); 

        /*
         * prints the load and store hits/misses and total cycle number
         */
        void printSummary(); 

        /*
         * loads data at an address from cache to CPU
         * if cache block doesn't exist, load from main memory
         * if cache set is full, evict based on replacement strategy
         * @param address the address we're trying to load from cache
         */
        void load(std::string address);

        /*
         * stores/writes data from CPU to cache or main memory
         * if cache block doesn't exist, load from main memory
         * if cache set is full, evict based on replacement strategy
         * writes to cache or main memory based on write-hit strategy
         * modify cache or not based on write-miss strategy
         * @param address the address we're trying to write to cache
         */
        void store(std::string address);

        /*
         * a helper method for evicting a block when cache is full
         * choose the block to evict based on replacement strategy
         * @param strategy lru or fifo
         * @param index the index of the set to evict a block from
         * returns the dirty bit of the evicted block
         */
        bool evict(std::string strategy, std::string index);

        /*
         * a helper method for to find the block to write to in a 
         * particular set
         * @param tag the tag of the address
         * @param index the index of the set to find the block
         * returns true on a hit, false otherwise
         */
        bool findBlock(std::string tag, std::string index); 

};



#endif // __CSIM_H__
