
@3030
D=A
@SP
M=M+1
A=M-1
M=D

@0
D=A
@pointer
D=D+A

@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D

@SP
M=M-1

@3040
D=A
@SP
M=M+1
A=M-1
M=D

@1
D=A
@pointer
D=D+A

@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D

@SP
M=M-1

@32
D=A
@SP
M=M+1
A=M-1
M=D

@2
D=A
@this
D=D+A

@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D

@SP
M=M-1

@46
D=A
@SP
M=M+1
A=M-1
M=D

@6
D=A
@that
D=D+A

@R13
M=D
@SP
A=M-1
D=M
@R13
A=M
M=D

@SP
M=M-1

@0
D=A
@pointer
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1

@1
D=A
@pointer
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
@SP
A=M-1
D=M
A=A-1
D=D+M
@SP
M=M-1
A=M-1
M=D

@2
D=A
@this
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
@SP
A=M-1
D=M
A=A-1
D=D-M
@SP
M=M-1
A=M-1
M=D

@6
D=A
@that
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1
@SP
A=M-1
D=M
A=A-1
D=D+M
@SP
M=M-1
A=M-1
M=D
(INFINITE_LOOP)
@INFINITE_LOOP
0;JMP
