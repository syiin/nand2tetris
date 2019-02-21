// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

//define x and save R0 to it
@R0 
D = M 
@x
M = D
  
//define y and save R1 to it
@R1
D = M
@y
M = D  

//define i and set it to y
@i
M = D

//define a result register
@result
M = 0

//Check for x or y = 0
@y
D = M
@END
D; JEQ
@x
D = M
@END
D; JEQ

(LOOP)
  @x
  D = M

  @result
  M = M + D
  
  @i
  M = M - 1
  D = M
  @LOOP
  D;JGT

(END)
  @result
  D = M
  @R2
  M = D
  @END
  0; JMP
