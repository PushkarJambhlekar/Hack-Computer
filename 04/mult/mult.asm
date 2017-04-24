// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

    @R2
    M=0

    @R1
    D=M
    @val2
    M=D

    @index
    M=0

(LOOP)
    @R0
    D=M
    @R2
    M=M+D
    @val2
    D=M-1
    M=D
    @LOOP
    D;JGT
(END)
    @END
    0;JMP
