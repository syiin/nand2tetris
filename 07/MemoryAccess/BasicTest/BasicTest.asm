
@10
D=A
@SP
M=M+1
A=M-1
M=D

@0
D=A
@LCL
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@21
D=A
@SP
M=M+1
A=M-1
M=D

@22
D=A
@SP
M=M+1
A=M-1
M=D

@2
D=A
@ARG
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@1
D=A
@ARG
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@36
D=A
@SP
M=M+1
A=M-1
M=D

@6
D=A
@THIS
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@42
D=A
@SP
M=M+1
A=M-1
M=D

@45
D=A
@SP
M=M+1
A=M-1
M=D

@5
D=A
@THAT
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@2
D=A
@THAT
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@510
D=A
@SP
M=M+1
A=M-1
M=D

@6
D=A
@5
D=D+A

@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D

@SP
M=M-1

@0
D=A
@LCL
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1

@5
D=A
@THAT
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

@1
D=A
@ARG
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
@THIS
D=D+A

A=D
D=M
@SP
A=M
M=D

@SP
M=M+1

@6
D=A
@THIS
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
@5
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
