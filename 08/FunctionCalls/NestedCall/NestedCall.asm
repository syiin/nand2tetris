//Write: Sys.init
(Sys.init)
@4000
D=A
@SP
M=M+1
A=M-1
M=D

@0
D=A
@3
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
@5000
D=A
@SP
M=M+1
A=M-1
M=D

@1
D=A
@3
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
//CallSys.main
@Sys.mainret$.0
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@LCL
M=D
@5
D=D-A
@ARG
M=D
@Sys.main
0;JMP
(Sys.mainret$.0)
//EndCallSys.main

@1
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
(LOOP)
@LOOP
0; JMP
//Write: Sys.main
(Sys.main)
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
D=0
@SP
M=M+1
A=M-1
M=D
@4001
D=A
@SP
M=M+1
A=M-1
M=D

@0
D=A
@3
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
@5001
D=A
@SP
M=M+1
A=M-1
M=D

@1
D=A
@3
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
@200
D=A
@SP
M=M+1
A=M-1
M=D

@1
D=A
@LCL
D=D+M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
@40
D=A
@SP
M=M+1
A=M-1
M=D

@2
D=A
@LCL
D=D+M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
@6
D=A
@SP
M=M+1
A=M-1
M=D

@3
D=A
@LCL
D=D+M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
@123
D=A
@SP
M=M+1
A=M-1
M=D
//CallSys.add12
@Sys.add12ret$.1
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@SP
D=M
@LCL
M=D
@6
D=D-A
@ARG
M=D
@Sys.add12
0;JMP
(Sys.add12ret$.1)
//EndCallSys.add12

@0
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

@2
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

@3
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

@4
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
//RETURN
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
//ENDRETURN
//Write: Sys.add12
(Sys.add12)
@4002
D=A
@SP
M=M+1
A=M-1
M=D

@0
D=A
@3
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
@5002
D=A
@SP
M=M+1
A=M-1
M=D

@1
D=A
@3
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
@12
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
//RETURN
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
//ENDRETURN