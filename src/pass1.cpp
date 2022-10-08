#include <iostream>
#include <string>

#include "pass1.hpp"

Pass1::Pass1(std::string input) {
    lex = new Lex(input);
}

std::map<std::string, int> Pass1::run() {
    std::map<std::string, int> labels;
    int lc = 0;     // The location counter
    
    Token token = lex->getNext();
    while (token.type != Eof) {
        switch (token.type) {
            case Id: {
                std::string label = token.id;
                labels[label] = lc;
                
                token = lex->getNext();
                if (token.type != Colon) {
                    std::cerr << "Error: Expected \':\' after label." << std::endl;
                    return labels;
                }
            }
            
            case Nl: break;
        
            // If not an ID, read and increment the location counter
            default: {
                while (token.type != Eof && token.type != Nl) {
                    token = lex->getNext();
                }
                lc += 4;
            }
        }
        
        token = lex->getNext();
    }
    
    return labels;
}

