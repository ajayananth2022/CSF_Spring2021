 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#include <iostream>
#include "csim.h"

using std::cout;
using std::endl;
using std::string; 

struct SimulationParams {
    int num_sets;
    int num_blocks;
    int size_blocks;
    string write_miss;
    string write_hit;
    string replace_strategy;
};

bool checkPowerTwo(int num) {
    if (num & (num - 1) == 0) {
        return true; 
    }
    return false; 
}

SimulationParams* parse_args(int argc, char *argv[]) {

    SimulationParams *args = new SimulationParams();

    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int size_blocks = atoi(argv[3]);
    string write_miss = argv[4];
    string write_hit = argv[5];
    string replace_strategy = argv[6];

    if (argc != 7) {
	cout << "Invalid number of arguments!" << endl;
	return NULL;
    }

    if (num_sets <= 0 || !checkPowerTwo(num_sets)) {
	cout << "Invalid number of sets in the cache!" << endl;
	return NULL;
    }
    if (num_blocks <= 0 || !checkPowerTwo(num_blocks)) {
	cout << "Invalid number of blocks in the cache!" << endl;
    return NULL;
    }
    if (size_blocks < 4 || !checkPowerTwo(size_blocks)) {
	cout << "Invalid size of blocks in the cache!" << endl;
	return NULL;
    }
    if (write_miss != "write-allocate" && write_miss != "no-write-allocate") {
	return NULL;
    }
    if (write_hit != "lru" && write_hit != "fifo") {
	return NULL;
    }

    args->num_sets = num_sets; 
    args->num_blocks = num_blocks; 
    args->size_blocks = size_blocks; 
    args->write_miss = write_miss; 
    args->write_hit = write_hit; 
    args->replace_strategy = replace_strategy; 

    return args; 
}

int main(int argc, char **argv) {
  struct SimulationParams *params = parse_args(argc, argv);
  if (!params) {
    fprintf(stderr, "Invalid simulation parameters\n");
    return 1;
  }
  // proceed with the actual simulation...
  return 0; 

}


