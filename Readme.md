## RISC-V Assembler

This is a simple RISC-V assembler written completely in C++. This assembler is currently made for a school project, but it can very easily be the base of more advanced projects.

The assembler contains two passes. Pass 1 reads the source file and determines the locations of all the labels (which is really easy given that all RISC-V instructions are the same length). This information is sent to Pass 2, which reads the source file again and generates the final binary. Pass 1 and 2 share the lexical analyzer, which simply returns the stream of tokens in the file.

This assembler supports all RV32I base instructions except FENCE, ECALL, and EBREAK. Note that it currently does not generate any executable formats beyond a flat binary. That said, converting it to support ELF should be very easy provided you already have an ELF generator.

