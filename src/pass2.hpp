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
protected:
    void build_r(TokenType opcode);
    void build_i(TokenType opcode);
    void build_load(TokenType opcode);
    void build_store(TokenType opcode);
    void build_br(TokenType opcode);
    void build_uj(TokenType opcode);
    int getRegister(TokenType token);
    int getALU(TokenType token);
    void checkComma();
    void checkNL();
private:
    Lex *lex;
    FILE *file;
    std::map<std::string, int> labels;
    int lc = 0;
};

