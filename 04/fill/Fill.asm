// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

  //32 16-bit words x 256 height


(KEYOFF)
//reset the counter and cursor
  @8192
  D = A
  @i
  M = D
  @SCREEN
  D=A
  @cursor
  M=D
(WHT)
//*cursor = 0
  @cursor
  A = M
  M = 0
//cursor = cursor + 1
  @cursor
  D = M
  @1
  D = D + A
  @cursor
  M = D
//i--
  @i
  MD = M - 1
//if (i > 0)
  @WHT
  D; JGT

 //listener
  @KBD
  D = M
  @KEYPRESS
  D; JNE
  @KEYOFF
  D; JEQ


(KEYPRESS)
//reset the counter & cursor
  @8192
  D = A
  @i
  M = D
  @SCREEN
  D=A
  @cursor
  M=D
(BLK)
  @cursor
  A = M
  M = -1
  @cursor
  D = M
  @1
  D = D + A
  @cursor
  M = D
  @i
  MD = M - 1
  @BLK
  D; JGT

  //listener
  @KBD
  D = M
  @KEYPRESS
  D; JNE
  @KEYOFF
  D; JEQ