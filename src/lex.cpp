#include <iostream>
#include <cctype>

#include "lex.hpp"

//
// Setups the lexical analyzer
//
Lex::Lex(std::string input) {
    reader = std::ifstream(input);
    if (reader.is_open()) {}
}

//
// Gets the next token in the stream
//
Token Lex::getNext() {
    if (!stack.empty()) {
        Token token = stack.top();
        stack.pop();
        return token;
    }

    Token token;
    token.type = Eof;
    if (reader.eof()) {
        return token;
    }
    
    while (!reader.eof()) {
        char c = reader.get();
        if (c == ' ' || isSymbol(c)) {
            if (isSymbol(c)) {
                if (buffer.length() == 0) {
                    token.type = getSymbol(c);
                    return token;
                } else {
                    Token token2;
                    token2.type = getSymbol(c);
                    stack.push(token2);
                }
            }
            
            if (buffer.length() == 0) continue;
            
            if (isKeyword()) {
                token.type = getKeyword();
                buffer = "";
                return token;
            } else if (isInt()) {
                token.type = Imm;
                token.imm = std::stoi(buffer);
                buffer = "";
                return token;
            } else {
                // TODO: Id
            }
        } else {
            buffer += c;
        }  
    }
    
    return token;
}

//
// Utility helper functions
//
bool Lex::isSymbol(char c) {
    switch (c) {
        case '\n':
        case ',':
        case '(':
        case ')': return true;
        
        default: {}
    }
    return false;
}

bool Lex::isKeyword() {
    if (buffer == "add") return true;
    else if (buffer == "sub") return true;
    else if (buffer == "sll") return true;
    else if (buffer == "slt") return true;
    else if (buffer == "sltu") return true;
    else if (buffer == "xor") return true;
    else if (buffer == "srl") return true;
    else if (buffer == "sra") return true;
    else if (buffer == "or") return true;
    else if (buffer == "and") return true;
    
    else if (buffer == "addi") return true;
    else if (buffer == "slli") return true;
    else if (buffer == "slti") return true;
    else if (buffer == "sltiu") return true;
    else if (buffer == "xori") return true;
    else if (buffer == "srli") return true;
    else if (buffer == "srai") return true;
    else if (buffer == "ori") return true;
    else if (buffer == "andi") return true;
    
    else if (buffer == "lb") return true;
    else if (buffer == "lh") return true;
    else if (buffer == "lw") return true;
    else if (buffer == "lbu") return true;
    else if (buffer == "lhu") return true;
    
    else if (buffer == "sb") return true;
    else if (buffer == "sh") return true;
    else if (buffer == "sw") return true;
    
    else if (buffer == "beq") return true;
    else if (buffer == "bne") return true;
    else if (buffer == "blt") return true;
    else if (buffer == "bge") return true;
    else if (buffer == "bltu") return true;
    else if (buffer == "bgeu") return true;
    
    else if (buffer == "lui") return true;
    else if (buffer == "auipc") return true;
    else if (buffer == "jal") return true;
    else if (buffer == "jalr") return true;
    
    else if (buffer == "x0") return true;
    else if (buffer == "x1") return true;
    else if (buffer == "x2") return true;
    else if (buffer == "x3") return true;
    else if (buffer == "x4") return true;
    else if (buffer == "x5") return true;
    else if (buffer == "x6") return true;
    else if (buffer == "x7") return true;
    else if (buffer == "x8") return true;
    else if (buffer == "x9") return true;
    else if (buffer == "x10") return true;
    else if (buffer == "x11") return true;
    else if (buffer == "x12") return true;
    else if (buffer == "x13") return true;
    else if (buffer == "x14") return true;
    else if (buffer == "x15") return true;
    else if (buffer == "x16") return true;
    else if (buffer == "x17") return true;
    else if (buffer == "x18") return true;
    else if (buffer == "x19") return true;
    else if (buffer == "x20") return true;
    else if (buffer == "x21") return true;
    else if (buffer == "x22") return true;
    else if (buffer == "x23") return true;
    else if (buffer == "x24") return true;
    else if (buffer == "x25") return true;
    else if (buffer == "x26") return true;
    else if (buffer == "x27") return true;
    else if (buffer == "x28") return true;
    else if (buffer == "x29") return true;
    else if (buffer == "x30") return true;
    else if (buffer == "x31") return true;
    
    return false;
}

bool Lex::isInt() {
    for (char c : buffer) {
        if (!isdigit(c)) return false;
    }
    return true;
}

TokenType Lex::getSymbol(char c) {
    switch (c) {
        case '\n': return Nl;
        case ',': return Comma;
        case '(': return LParen;
        case ')': return RParen;
        
        default: {}
    }
    return None;
}

TokenType Lex::getKeyword() {
    if (buffer == "add") return Add;
    else if (buffer == "sub") return Sub;
    else if (buffer == "sll") return Sll;
    else if (buffer == "slt") return Slt;
    else if (buffer == "sltu") return Sltu;
    else if (buffer == "xor") return Xor;
    else if (buffer == "srl") return Srl;
    else if (buffer == "sra") return Sra;
    else if (buffer == "or") return Or;
    else if (buffer == "and") return And;
    
    else if (buffer == "addi") return Addi;
    else if (buffer == "slli") return Slli;
    else if (buffer == "slti") return Slti;
    else if (buffer == "sltiu") return Sltiu;
    else if (buffer == "xori") return Xori;
    else if (buffer == "srli") return Srli;
    else if (buffer == "srai") return Srai;
    else if (buffer == "ori") return Ori;
    else if (buffer == "andi") return Andi;
    
    else if (buffer == "lb") return Lb;
    else if (buffer == "lh") return Lh;
    else if (buffer == "lw") return Lw;
    else if (buffer == "lbu") return Lbu;
    else if (buffer == "lhu") return Lhu;
    
    else if (buffer == "sb") return Sb;
    else if (buffer == "sh") return Sh;
    else if (buffer == "sw") return Sw;
    
    else if (buffer == "beq") return Beq;
    else if (buffer == "bne") return Bne;
    else if (buffer == "blt") return Blt;
    else if (buffer == "bge") return Bge;
    else if (buffer == "bltu") return Bltu;
    else if (buffer == "bgeu") return Bgeu;
    
    else if (buffer == "lui") return Lui;
    else if (buffer == "auipc") return Auipc;
    else if (buffer == "jal") return Jal;
    else if (buffer == "jalr") return Jalr;
    
    else if (buffer == "x0") return X0;
    else if (buffer == "x1") return X1;
    else if (buffer == "x2") return X2;
    else if (buffer == "x3") return X3;
    else if (buffer == "x4") return X4;
    else if (buffer == "x5") return X5;
    else if (buffer == "x6") return X6;
    else if (buffer == "x7") return X7;
    else if (buffer == "x8") return X8;
    else if (buffer == "x9") return X9;
    else if (buffer == "x10") return X10;
    else if (buffer == "x11") return X11;
    else if (buffer == "x12") return X12;
    else if (buffer == "x13") return X13;
    else if (buffer == "x14") return X14;
    else if (buffer == "x15") return X15;
    else if (buffer == "x16") return X16;
    else if (buffer == "x17") return X17;
    else if (buffer == "x18") return X18;
    else if (buffer == "x19") return X19;
    else if (buffer == "x20") return X20;
    else if (buffer == "x21") return X21;
    else if (buffer == "x22") return X22;
    else if (buffer == "x23") return X23;
    else if (buffer == "x24") return X24;
    else if (buffer == "x25") return X25;
    else if (buffer == "x26") return X26;
    else if (buffer == "x27") return X27;
    else if (buffer == "x28") return X28;
    else if (buffer == "x29") return X29;
    else if (buffer == "x30") return X30;
    else if (buffer == "x31") return X31;
    return None;
}

//
// The debug function for tokens
//
void Token::print() {
    switch (type) {
        case Eof: std::cout << "EOF "; break;
        
        case Add: std::cout << "add "; break;
        case Sub: std::cout << "sub "; break;
        case Sll: std::cout << "sll "; break;
        case Slt: std::cout << "slt "; break;
        case Sltu: std::cout << "sltu "; break;
        case Xor: std::cout << "xor "; break;
        case Srl: std::cout << "srl "; break;
        case Sra: std::cout << "sra "; break;
        case Or: std::cout << "or "; break;
        case And: std::cout << "and "; break;
        
        case Addi: std::cout << "addi "; break;
        case Slli: std::cout << "slli "; break;
        case Slti: std::cout << "slti "; break;
        case Sltiu: std::cout << "sltiu "; break;
        case Xori: std::cout << "xori "; break;
        case Srli: std::cout << "srli "; break;
        case Srai: std::cout << "srai "; break;
        case Ori: std::cout << "ori "; break;
        case Andi: std::cout << "andi "; break;
        
        case Lb: std::cout << "lb "; break;
        case Lh: std::cout << "lh "; break;
        case Lw: std::cout << "lw "; break;
        case Lbu: std::cout << "lbu "; break;
        case Lhu: std::cout << "lhu "; break;
        
        case Sb: std::cout << "sb "; break;
        case Sh: std::cout << "sh "; break;
        case Sw: std::cout << "sw "; break;
        
        case Beq: std::cout << "beq "; break;
        case Bne: std::cout << "bne "; break;
        case Blt: std::cout << "blt "; break;
        case Bge: std::cout << "bge "; break;
        case Bltu: std::cout << "bltu "; break;
        case Bgeu: std::cout << "bgeu "; break;
        
        case Lui: std::cout << "lui "; break;
        case Auipc: std::cout << "auipc "; break;
        case Jal: std::cout << "jal "; break;
        case Jalr: std::cout << "jalr "; break;
        
        case X0: std::cout << "x0 "; break;
        case X1: std::cout << "x1 "; break;
        case X2: std::cout << "x2 "; break;
        case X3: std::cout << "x3 "; break;
        case X4: std::cout << "x4 "; break;
        case X5: std::cout << "x5 "; break;
        case X6: std::cout << "x6 "; break;
        case X7: std::cout << "x7 "; break;
        case X8: std::cout << "x8 "; break;
        case X9: std::cout << "x9 "; break;
        case X10: std::cout << "x10 "; break;
        case X11: std::cout << "x11 "; break;
        case X12: std::cout << "x12 "; break;
        case X13: std::cout << "x13 "; break;
        case X14: std::cout << "x14 "; break;
        case X15: std::cout << "x15 "; break;
        case X16: std::cout << "x16 "; break;
        case X17: std::cout << "x17 "; break;
        case X18: std::cout << "x18 "; break;
        case X19: std::cout << "x19 "; break;
        case X20: std::cout << "x20 "; break;
        case X21: std::cout << "x21 "; break;
        case X22: std::cout << "x22 "; break;
        case X23: std::cout << "x23 "; break;
        case X24: std::cout << "x24 "; break;
        case X25: std::cout << "x25 "; break;
        case X26: std::cout << "x26 "; break;
        case X27: std::cout << "x27 "; break;
        case X28: std::cout << "x28 "; break;
        case X29: std::cout << "x29 "; break;
        case X30: std::cout << "x30 "; break;
        case X31: std::cout << "x31 "; break;
        
        case Nl: std::cout << std::endl; break;
        case Comma: std::cout << ", "; break;
        case LParen: std::cout << "("; break;
        case RParen: std::cout << ")"; break;
        
        case Imm: std::cout << imm << " "; break;
        
        default: std::cout << "?? ";
    }
}

