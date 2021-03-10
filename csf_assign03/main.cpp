 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#include <iostream>
#include "csim.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
    if (argc < 7) {
	cout << "Too few arguments!" << endl;
	return 1;
    }
    int num_sets = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int size_blocks = atoi(argv[3]);
    char* write_miss = argv[4];
    char* write_hit = argv[5];
    char* replace_strategy = argv[6];
    if (num_sets <= 0) {
	cout << "Invalid number of sets in the cache!" << endl;
	return 2;
    }
    if (num_blocks <= 0) {
	cout << "Invalid number of blocks in the cache!" << endl;
        return 2;
    }
    if (size_blocks < 4) {
	cout << "Invalid size of blocks in the cache!" << endl;
	return 2;
    }
    return 0;
}
