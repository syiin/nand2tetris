#ifndef CODE_H
#define CODE_H

#include <fstream>
#include <map>
#include <string>

class CodeWriter
{

private:
  std::map<std::string, std::string> arithTable;
  std::map<std::string, std::string> memNameTable;
  std::string fileName;
  std::ofstream myfile;

public:
  CodeWriter(std::string outputName)
  {
    fileName = outputName;
    myfile.open(fileName);
    initTables();
  };

  std::string writeArithmetic(std::string theCommand)
  {
    std::string outputString = "";
    outputString = outputString + arithTable[theCommand];

    myfile << outputString;
    return arithTable[theCommand];
  };

  std::string writePushPop(std::string theCommand, std::string segment, int index)
  {

    if (theCommand == "C_POP")
    {
      myfile << popSegToD(index, segment);
      myfile << "\n@R13\nM=D\n";
      myfile << popStackToD();
      myfile << "@R13\nA=M\nM=D\n";
      myfile << decrementStackPointer();
      return popSegToD(index, segment) + "\n@R13\nM=D\n@SP\nA=M-1\nD=M\n@R13\nA=M\nM=D\n" + decrementStackPointer();
    }
    else if (theCommand == "C_PUSH")
    {
      if (segment == "constant")
      {
        myfile << "\n@" + std::to_string(index) + "\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n";
        return "\n@" + std::to_string(index) + "\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n";
      }
      else
      {
        myfile << popSegToD(index, segment) + "\nA=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
        return popSegToD(index, segment) + "A=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
      }
    }
  };

  std::string popStackToD()
  {
    return "@SP\nM=M-1\nA=M\nD=M\n";
  };

  std::string popSegToD(int idx, std::string segment)
  {
    return "\n@" + std::to_string(idx) + "\nD=A\n@" + memNameTable[segment] + "\nD=D+A\n";
  }

  std::string decrementStackPointer()
  {
    return "\n@SP\nM=M-1\n";
  }

  std::string incrementStackPointer()
  {
    return "\n@SP\nM=M+1\n";
  }

  void close()
  {
    myfile << "(INFINITE_LOOP)\n@INFINITE_LOOP\n0;JMP\n";
    myfile.close();
  }

  void initTables()
  {
    std::string decrementSP = "@SP\nM=M-1\nA=M-1\nM=D\n";
    arithTable["add"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D+M\n" + decrementSP;
    arithTable["sub"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\n" + decrementSP;
    arithTable["neg"] = "@SP\nA=M-1\nM=-M\n";

    std::string failCase = "@SP\nA=M-1\nA=A-1\nM=0\n@CONTINUE\n0;JMP\n";
    std::string successCase = "(SUCCESS)\n@SP\nA=M-1\nA=A-1\nM=1\n";

    arithTable["eq"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=M-D\n@SUCCESS\nD; JEQ\n" + failCase + successCase + "(CONTINUE)\n" + decrementSP;
    arithTable["gt"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=M-D\n@SUCCESS\nD; JGT\n" + failCase + successCase + "(CONTINUE)\n" + decrementSP;

    arithTable["lt"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\nD; JLT\n";

    arithTable["and"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D|M\n" + decrementSP;
    arithTable["or"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D&M\n" + decrementSP;
    arithTable["not"] = "@SP\nA=M-1\nA=A-1\nD=!M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";

    memNameTable["local"] = "LCL";
    memNameTable["argument"] = "ARG";
    memNameTable["this"] = "THIS";
    memNameTable["that"] = "THAT";
    memNameTable["pointer"] = "3";
    memNameTable["temp"] = "5";
    memNameTable["static"] = "16";
  }
};

#endif

/*

add
---------------
@SP 
A=M-1
D=M     //load y into D
A=A-1   //address into x
D=D+M   //save x+y into D
@SP
M=M-1   //SP--
A=M-1   //access location below SP (ie. where x is)
M=D     //save into x the x+y stored in D

==============================================
push constant i
---------------
@i
D=A
@SP
M=M+1
A=M-1
M=D

pop temp i
---------------
@i
D=A
@segment
D=D+A 

@temp
M=D

@SP
M=M-1
D=M

@temp
A=M
M=D

push segment i
---------------
@i
D=A
@segment
D=D+A 
A=D
D=M

@SP
A=M
M=D

@SP
M=M+1














*/