(SimpleFunction.test)
D=0
@SP
M=M+1
A=M-1
M=D
D=0
@SP
M=M+1
A=M-1
M=D

@0
D=A
@LCL
D=D+M

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1

@1
D=A
@LCL
D=D+M

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
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
@SP
A=M-1
M=!M

@0
D=A
@ARG
D=D+M

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
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

@1
D=A
@ARG
D=D+M

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
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
@LCL
D=M
@R13
M=D
@R13
D=M
@5
D=D-A
A=D
D=M
@R14
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@R13

D=M
@1
D=D-A
A=D
D=M
@THAT
M=D
@R13

D=M
@2
D=D-A
A=D
D=M
@THIS
M=D
@R13

D=M
@3
D=D-A
A=D
D=M
@ARG
M=D
@R13

D=M
@4
D=D-A
A=D
D=M
@LCL
M=D
@R14
A=M
0;JMP
