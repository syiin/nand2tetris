
@17
D=A
@SP
M=M+1
A=M-1
M=D

@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line1
D; JEQ
@SP
A=M-1
M=0
@CONTINUE.line1
0;JMP
(SUCCESS.line1)
@SP
A=M-1
M=-1
(CONTINUE.line1)

@17
D=A
@SP
M=M+1
A=M-1
M=D

@16
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line3
D; JEQ
@SP
A=M-1
M=0
@CONTINUE.line3
0;JMP
(SUCCESS.line3)
@SP
A=M-1
M=-1
(CONTINUE.line3)

@16
D=A
@SP
M=M+1
A=M-1
M=D

@17
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line5
D; JEQ
@SP
A=M-1
M=0
@CONTINUE.line5
0;JMP
(SUCCESS.line5)
@SP
A=M-1
M=-1
(CONTINUE.line5)

@892
D=A
@SP
M=M+1
A=M-1
M=D

@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line7
D; JLT
@SP
A=M-1
M=0
@CONTINUE.line7
0;JMP
(SUCCESS.line7)
@SP
A=M-1
M=-1
(CONTINUE.line7)

@891
D=A
@SP
M=M+1
A=M-1
M=D

@892
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line9
D; JLT
@SP
A=M-1
M=0
@CONTINUE.line9
0;JMP
(SUCCESS.line9)
@SP
A=M-1
M=-1
(CONTINUE.line9)

@891
D=A
@SP
M=M+1
A=M-1
M=D

@891
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line11
D; JLT
@SP
A=M-1
M=0
@CONTINUE.line11
0;JMP
(SUCCESS.line11)
@SP
A=M-1
M=-1
(CONTINUE.line11)

@32767
D=A
@SP
M=M+1
A=M-1
M=D

@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line13
D; JGT
@SP
A=M-1
M=0
@CONTINUE.line13
0;JMP
(SUCCESS.line13)
@SP
A=M-1
M=-1
(CONTINUE.line13)

@32766
D=A
@SP
M=M+1
A=M-1
M=D

@32767
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line15
D; JGT
@SP
A=M-1
M=0
@CONTINUE.line15
0;JMP
(SUCCESS.line15)
@SP
A=M-1
M=-1
(CONTINUE.line15)

@32766
D=A
@SP
M=M+1
A=M-1
M=D

@32766
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M
@SP
A=M-1
D=M-D
@SUCCESS.line17
D; JGT
@SP
A=M-1
M=0
@CONTINUE.line17
0;JMP
(SUCCESS.line17)
@SP
A=M-1
M=-1
(CONTINUE.line17)

@57
D=A
@SP
M=M+1
A=M-1
M=D

@31
D=A
@SP
M=M+1
A=M-1
M=D

@53
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M

@SP
M=M-1
A=M
D=M+D
@SP
M=M+1
A=M-1
M=D

@112
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M

@SP
M=M-1
A=M
D=M-D
@SP
M=M+1
A=M-1
M=D
@SP
A=M-1
M=-M
@SP
M=M-1
A=M
D=M

@SP
A=M-1
D=D&M
M=D

@82
D=A
@SP
M=M+1
A=M-1
M=D
@SP
M=M-1
A=M
D=M

@SP
A=M-1
D=D|M
M=D
@SP
A=M-1
M=!M

(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
