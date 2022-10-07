#include <cstdio>
#include <iostream>

#include "pass2.hpp"
#include "lex.hpp"

Pass2::Pass2(std::string input, std::string output) {
    lex = new Lex(input);
    file = fopen(output.c_str(), "wb");
}

void Pass2::run() {
    Token token = lex->getNext();
    while (token.type != Eof) {
        switch (token.type) {
            // R-Type
            case Add:
            case Sub:
            case Sll:
            case Slt:
            case Sltu:
            case Xor:
            case Srl:
            case Sra:
            case Or:
            case And: build_r(token.type); break;
            
            default: {}
        }
        
        token = lex->getNext();
    }
    
    
    // Close everything
    delete lex;
    fclose(file);
}

//
// Builds R-Type instructions
//
void Pass2::build_r(TokenType opcode) {
    // Get each token
    int rd, rs1, rs2;
    Token token = lex->getNext();
    rd = getRegister(token.type);
    if (rd == -1) {
        std::cerr << "Invalid token: Expected register." << std::endl;
        return;
    }
    
    checkComma();
    
    token = lex->getNext();
    rs1 = getRegister(token.type);
    if (rs1 == -1) {
        std::cerr << "Invalid token: Expected register source 1." << std::endl;
        return;
    }
    
    checkComma();
    
    token = lex->getNext();
    rs2 = getRegister(token.type);
    if (rs2 == -1) {
        std::cerr << "Invalid token: Expected register source 2." << std::endl;
        return;
    }
    
    checkNL();
    
    // Get the ALU operand
    int func3 = getALU(opcode);
    
    uint32_t func7 = 0;
    if (opcode == Sub || opcode == Sra) {
        func7 = 32;
    }

    // Encode the instruction
    uint32_t instr = 0;
    
    instr |= (uint32_t)(0b0110011);     // I-Type opcode
    instr |= (uint32_t)(rd << 7);
    instr |= (uint32_t)(func3 << 12);
    instr |= (uint32_t)(rs1 << 15);
    instr |= (uint32_t)(rs2 << 20);
    instr |= (uint32_t)(func7 << 25);
    
    printf("%X\n", instr);
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Translates a register token to an integer
//
int Pass2::getRegister(TokenType token) {
    switch (token) {
        case X0: return 0;
        case X1: return 1;
        case X2: return 2;
        case X3: return 3;
        case X4: return 4;
        case X5: return 5;
        case X6: return 6;
        case X7: return 7;
        case X8: return 8;
        case X9: return 9;
        case X10: return 10;
        case X11: return 11;
        case X12: return 12;
        case X13: return 13;
        case X14: return 14;
        case X15: return 15;
        case X16: return 16;
        case X17: return 17;
        case X18: return 18;
        case X19: return 19;
        case X20: return 20;
        case X21: return 21;
        case X22: return 22;
        case X23: return 23;
        case X24: return 24;
        case X25: return 25;
        case X26: return 26;
        case X27: return 27;
        case X28: return 28;
        case X29: return 29;
        case X30: return 30;
        case X31: return 31;
        
        default: {}
    }
    
    return -1;
}

//
// Translates an ALU operand
//
int Pass2::getALU(TokenType token) {
    switch (token) {
        case Add:
        case Sub: return 0b000;
        
        case Sll: return 0b001;
        
        case Slt: return 0b010;
        
        case Sltu: return 0b011;
        
        case Xor: return 0b100;
        
        case Srl:
        case Sra: return 0b101;
        
        case Or: return 0b110;
        
        case And: return 0b111;
        
        default: {}
    }
    
    return 0;
}

//
// A helpful syntax utility function
//
void Pass2::checkComma() {
    Token token = lex->getNext();
    if (token.type != Comma) {
        std::cerr << "Error: Expected \',\'." << std::endl;
        return;
    }
}

void Pass2::checkNL() {
    Token token = lex->getNext();
    if (token.type != Nl) {
        std::cerr << "Error: Expected newline." << std::endl;
        return;
    }
}

