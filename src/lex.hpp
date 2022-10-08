#pragma once

#include <string>
#include <fstream>
#include <stack>

//
// Defines RISC-V tokens
//
enum TokenType {
    None,
    Eof,
    
    // R-Type instructions
    Add, Sub,
    Sll,
    Slt,
    Sltu,
    Xor,
    Srl, Sra,
    Or,
    And,
    
    // I-Type instructions
    Addi,
    Slli,
    Slti,
    Sltiu,
    Xori,
    Srli, Srai,
    Ori,
    Andi,
    
    // Load instructions
    Lb, Lh, Lw, Lbu, Lhu,
    
    // Store instructions
    Sb, Sh, Sw,
    
    // Branch instructions
    Beq, Bne, Blt, Bge, Bltu, Bgeu,
    
    // Other instructions
    Lui, Auipc, Jal, Jalr,
    
    // Registers
    X0, X1, X2, X3, X4, X5, X6, X7, X8, X9, X10,
    X11, X12, X13, X14, X15, X16, X17, X18, X19, X20,
    X21, X22, X23, X24, X25, X26, X27, X28, X29, X30,
    X31,
    
    // Symbols
    Nl,
    Comma,
    LParen, RParen,
    Colon,
    
    Id,
    Imm
};

//
// Holds information on each token
//
struct Token {
    TokenType type = None;
    int imm = 0;
    std::string id = "";
    
    void print();
};

//
// The scanner class
//
class Lex {
public:
    explicit Lex(std::string input);
    Token getNext();
private:
    std::ifstream reader;
    std::string buffer = "";
    std::stack<Token> stack;
    
    bool isSymbol(char c);
    bool isKeyword();
    bool isInt();
    
    TokenType getSymbol(char c);
    TokenType getKeyword();
};

