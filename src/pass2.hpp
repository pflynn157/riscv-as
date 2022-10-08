#pragma once

#include <string>
#include <cstdio>

#include "lex.hpp"

class Pass2 {
public:
    explicit Pass2(std::string input, std::string output);
    void run();
protected:
    void build_r(TokenType opcode);
    void build_i(TokenType opcode);
    void build_load(TokenType opcode);
    void build_store(TokenType opcode);
    void build_br(TokenType opcode);
    int getRegister(TokenType token);
    int getALU(TokenType token);
    void checkComma();
    void checkNL();
private:
    Lex *lex;
    FILE *file;
};

