
// Prints back the first argument given to the program in hexadecimal.


#include <iostream>
#include "dump_to_hexadecimal.cpp"

void main(int argc, char ** argv) {
    std::cout << dump_to_hexadecimal(argv[1]);
}
