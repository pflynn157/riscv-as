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
            
            case Jalr:
            case Addi:
            case Slti:
            case Sltiu:
            case Slli:
            case Srli:
            case Srai:
            case Xori:
            case Ori:
            case Andi: build_i(token.type); break;
            
            case Lb:
            case Lh:
            case Lw:
            case Lbu:
            case Lhu: build_load(token.type); break;
            
            case Sb:
            case Sh:
            case Sw: build_store(token.type); break;
            
            case Beq:
            case Bne:
            case Blt:
            case Bge:
            case Bltu:
            case Bgeu: build_br(token.type); break;
            
            case Lui:
            case Auipc:
            case Jal: build_uj(token.type); break;
            
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
    
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Builds I-Type instructions
//
void Pass2::build_i(TokenType opcode) {
    // Get each token
    int rd, rs1, imm;
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
    imm = token.imm;
    if (token.type != Imm) {
        std::cerr << "Invalid token: Expected immediate for source 2." << std::endl;
        return;
    }
    
    checkNL();
    
    // Get the ALU operand
    int func3 = getALU(opcode);

    // Encode the instruction
    uint32_t instr = 0;
    
    if (opcode == Jalr) {
        instr |= (uint32_t)(0b1100111);     // (JALR) I-Type opcode
    } else {
        instr |= (uint32_t)(0b0010011);     // I-Type opcode
    }
    
    instr |= (uint32_t)(rd << 7);
    instr |= (uint32_t)(func3 << 12);
    instr |= (uint32_t)(rs1 << 15);
    
    // Encode the operand
    // This differs slightly with the shift instructions
    if (opcode == Slli || opcode == Srli || opcode == Srai) {
        uint8_t func7 = 0;
        if (opcode == Srai) func7 = 32;
        
        instr |= (uint32_t)(((uint8_t)imm) << 20);
        instr |= (uint32_t)(func7 << 25);
    } else {
        instr |= (uint32_t)(imm << 20);
    }
    
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Builds the load instructions
//
void Pass2::build_load(TokenType opcode) {
    // Get each token
    int rd, rs1, imm;
    Token token = lex->getNext();
    rd = getRegister(token.type);
    if (rd == -1) {
        std::cerr << "Invalid token: Expected register." << std::endl;
        return;
    }
    
    checkComma();
    
    token = lex->getNext();
    imm = token.imm;
    if (token.type != Imm) {
        std::cerr << "Invalid token: Expected offset." << std::endl;
        return;
    }
    
    token = lex->getNext();
    if (token.type != LParen) {
        std::cerr << "Invalid token: Expected \'(\'." << std::endl;
        return;
    }
    
    token = lex->getNext();
    rs1 = getRegister(token.type);
    if (rs1 == -1) {
        std::cerr << "Invalid token: expected offset register." << std::endl;
        return;
    }
    
    token = lex->getNext();
    if (token.type != RParen) {
        std::cerr << "Invalid token: Expected \')\'." << std::endl;
        return;
    }
    
    checkNL();
    
    // Encode func3
    int func3 = 0b010;
    switch (opcode) {
        case Lb: func3 = 0b000; break;
        case Lh: func3 = 0b001; break;
        case Lw: func3 = 0b010; break;
        case Lbu: func3 = 0b100; break;
        case Lhu: func3 = 0b101; break;
        
        default: {}
    }
    
    // Encode the instruction
    uint32_t instr = 0;
    
    instr |= (uint32_t)(0b0000011);     // Load-Type opcode
    instr |= (uint32_t)(rd << 7);
    instr |= (uint32_t)(func3 << 12);
    instr |= (uint32_t)(rs1 << 15);
    instr |= (uint32_t)(imm << 20);
    
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Builds store instructions
//
void Pass2::build_store(TokenType opcode) {
// Get each token
    int rd, rs1, imm;
    Token token = lex->getNext();
    rd = getRegister(token.type);
    if (rd == -1) {
        std::cerr << "Invalid token: Expected register." << std::endl;
        return;
    }
    
    checkComma();
    
    token = lex->getNext();
    imm = token.imm;
    if (token.type != Imm) {
        std::cerr << "Invalid token: Expected offset." << std::endl;
        return;
    }
    
    token = lex->getNext();
    if (token.type != LParen) {
        std::cerr << "Invalid token: Expected \'(\'." << std::endl;
        return;
    }
    
    token = lex->getNext();
    rs1 = getRegister(token.type);
    if (rs1 == -1) {
        std::cerr << "Invalid token: expected offset register." << std::endl;
        return;
    }
    
    token = lex->getNext();
    if (token.type != RParen) {
        std::cerr << "Invalid token: Expected \')\'." << std::endl;
        return;
    }
    
    checkNL();
    
    // Encode func3
    int func3 = 0b010;
    switch (opcode) {
        case Sb: func3 = 0b000; break;
        case Sh: func3 = 0b001; break;
        case Sw: func3 = 0b010; break;
        
        default: {}
    }
    
    // Encode the immediate
    uint8_t imm1 = (uint8_t)imm;
    uint8_t imm2 = (uint8_t)(imm >> 5);
    
    // Encode the instruction
    uint32_t instr = 0;
    
    instr |= (uint32_t)(0b0100011);     // Store-Type opcode
    instr |= (uint32_t)(imm1 << 7);
    instr |= (uint32_t)(func3 << 12);
    instr |= (uint32_t)(rs1 << 15);
    instr |= (uint32_t)(rd << 20);
    instr |= (uint32_t)(imm2 << 25);
    
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Builds the branch instructions
//
void Pass2::build_br(TokenType opcode) {
    // Get each token
    int rs1, rs2, imm;
    Token token = lex->getNext();
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
    
    checkComma();
    
    // TODO: This should be a label
    token = lex->getNext();
    imm = token.imm;
    if (token.type != Imm) {
        std::cerr << "Invalid token: Expected label." << std::endl;
        return;
    }
    
    checkNL();
    
    // Get the branch operand
    int func3 = 0;
    switch (opcode) {
        case Beq: func3 = 0; break;
        case Bne: func3 = 0b001; break;
        case Blt: func3 = 0b100; break;
        case Bge: func3 = 0b101; break;
        case Bltu: func3 = 0b110; break;
        case Bgeu: func3 = 0b111; break;
        
        default: {}
    }
    
    // Encode the immediate
    uint8_t imm1 = (uint8_t)((imm & 0x00800) >> 11);    // Bit 11
           imm1 |= (uint8_t)(imm & 0b00011110);         // Bit [4:1]
    uint8_t imm2 = (uint8_t)((imm & 0x007E0) >> 5);     // Bit [10:5]
           imm2 |= (uint8_t)((imm & 0x01000) >> 6);     // Bit 12

    // Encode the instruction
    uint32_t instr = 0;
    
    instr |= (uint32_t)(0b1100011);     // B-Type opcode
    instr |= (uint32_t)(imm1 << 7);
    instr |= (uint32_t)(func3 << 12);
    instr |= (uint32_t)(rs1 << 15);
    instr |= (uint32_t)(rs2 << 20);
    instr |= (uint32_t)(imm2 << 25);
    
    fwrite(&instr, sizeof(uint32_t), 1, file);
}

//
// Builds U/J-Type instructions
//
void Pass2::build_uj(TokenType opcode) {
    // Get each token
    int rd;
    uint32_t imm = 0;
    Token token = lex->getNext();
    rd = getRegister(token.type);
    if (rd == -1) {
        std::cerr << "Invalid token: Expected register." << std::endl;
        return;
    }
    
    checkComma();
    
    token = lex->getNext();
    imm = token.imm;
    if (token.type != Imm) {
        std::cerr << "Invalid token: Expected label." << std::endl;
        return;
    }
    
    checkNL();
    
    // If we have the JAL instruction, we need to do the fancy encoding
    if (opcode == Jal) {
        uint32_t imm1 = (uint32_t)((imm & 0x0FF000) >> 12);     // imm[19:12]    -> 0:8
                imm1 |= (uint32_t)((imm & 0x0800) >> 2);        // imm[11]       -> 9
                imm1 |= (uint32_t)((imm & 0x07FE) << 8);        // imm[10:1]     -> 10
                imm1 |= (uint32_t)((imm & 100000) >> 9);        // imm[20]       -> 11
        
        imm = imm1;
    }
    
    // Encode the instruction
    uint32_t instr = 0;
    
    if (opcode == Lui) {
        instr |= (uint32_t)(0b0110111);     // Lui U-Type opcode
    } else if (opcode == Auipc) {
        instr |= (uint32_t)(0b0010111);     // Auipc U-Type opcode
    } else if (opcode == Jal) {
        instr |= (uint32_t)(0b1101111);     // J-Type opcode
    }
    instr |= (uint32_t)(rd << 7);
    instr |= (uint32_t)(imm << 12);
    
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
        case Jalr:
        case Addi:
        case Add:
        case Sub: return 0b000;
        
        case Slli:
        case Sll: return 0b001;
        
        case Slti:
        case Slt: return 0b010;
        
        case Sltiu:
        case Sltu: return 0b011;
        
        case Xori:
        case Xor: return 0b100;
        
        case Srli: case Srai:
        case Srl:
        case Sra: return 0b101;
        
        case Ori:
        case Or: return 0b110;
        
        case Andi:
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


