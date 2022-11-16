#pragma once

#include <string>
#include <cstdio>
#include <map>

#include "lex.hpp"

class Pass2 {
public:
    explicit Pass2(std::string input, std::string output);
    void setMap(std::map<std::string, int> labels);
    void run();
    
    void setFormat(std::string format) { this->format = format; }
protected:
    void build_r(TokenType opcode);
    void build_i(TokenType opcode);
    void build_load(TokenType opcode);
    void build_store(TokenType opcode);
    void build_br(TokenType opcode);
    void build_uj(TokenType opcode);
    void build_fload(TokenType opcode);
    void build_fstore(TokenType opcode);
    void build_falu(TokenType opcode);
    int getRegister(TokenType token);
    int getFloatRegister(TokenType token);
    int getALU(TokenType token);
    std::string convertToBinary(uint32_t instr);
    void checkComma();
    void checkNL();
private:
    Lex *lex;
    FILE *file;
    std::map<std::string, int> labels;
    int lc = 0;
    
    // Formats: default, string
    std::string format = "default";
};

