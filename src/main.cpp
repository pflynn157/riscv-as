#include <iostream>
#include <string>

#include "pass2.hpp"

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cerr << "Error: No input file." << std::endl;
        return 1;
    }
    
    std::string input = argv[1];
    
    Pass2 *pass2 = new Pass2(input, "out");
    pass2->run();
    delete pass2;

    return 0;
}

