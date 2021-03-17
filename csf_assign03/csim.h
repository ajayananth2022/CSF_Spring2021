 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#ifndef __CSIM_H__
#define __CSIM_H__

#include <stdio.h>

bool checkPowerTwo(int num);

class Simulator {
    private:
        int num_sets;
        int num_blocks;
        int size_blocks;
        string write_miss;
        string write_hit;
        string replace_strategy;

    public:
        Simulator(int argc, char *argv[]); //constructor
};



#endif // __CSIM_H__
