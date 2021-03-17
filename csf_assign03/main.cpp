 //Lucy Zhang, Ajay Ananthakrishnan
 //wzhang99@jhu.edu, ajayananth@jhu.edu
#include <iostream>
#include "csim.h"

using std::cout;
using std::endl;
using std::string; 

int main(int argc, char **argv) {
  Simulator sim(argc, argv);
  if (!sim) {
    fprintf(stderr, "Invalid simulation parameters\n");
    return 1;
  }
  // proceed with the actual simulation...
  return 0; 

}


