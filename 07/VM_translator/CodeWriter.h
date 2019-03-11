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
  int lineIdx = 0;

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
    outputString = outputString + makeLogicString(theCommand);

    myfile << outputString;
    return makeLogicString(theCommand);
  };

  std::string writePushPop(std::string theCommand, std::string segment, int index)
  {

    if (theCommand == "C_POP")
    {
      if (segment == "static")
      {
        myfile << "\n@" + std::to_string(index) + "\nD=A\n@" + memNameTable[segment] + "\nD=D+A\n";
        myfile << "\n@R13\nM=D\n";
        myfile << popStackToD();
        myfile << "@R13\nA=M\nM=D\n";
      }
      else
      {
        myfile << popSegToD(index, segment);
        myfile << "\n@R13\nM=D\n";
        myfile << popStackToD();
        myfile << "@R13\nA=M\nM=D\n";
      }
      return popSegToD(index, segment) + "\n@R13\nM=D\n" + popStackToD() + "@R13\nA=M\nM=D\n";
    }
    else if (theCommand == "C_PUSH")
    {
      if (segment == "constant")
      {
        myfile << "\n@" + std::to_string(index) + "\nD=A\n" + pushDToStack();
        return "\n@" + std::to_string(index) + "\nD=A\n" + pushDToStack();
      }
      else
      {
        if (segment == "static")
        {
          myfile << "\n@" + std::to_string(index) + "\nD=A\n@" + memNameTable[segment] + "\nD=D+A\n";
          myfile << "\nA=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
          return popSegToD(index, segment) + "A=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
        }
        else
        {
          myfile << popSegToD(index, segment) + "\nA=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
          return popSegToD(index, segment) + "A=D\nD=M\n@SP\nA=M\nM=D\n" + incrementStackPointer();
        }
      }
    }
  };

  std::string makeLogicString(std::string theCommand)
  {
    lineIdx++;
    std::string lineString = "line" + std::to_string(lineIdx);

    if (theCommand == "add")
    {
      return popStackToD() + "\n@SP\nM=M-1\nA=M\nD=M+D\n" + pushDToStack();
    }
    else if (theCommand == "sub")
    {
      return popStackToD() + "\n@SP\nM=M-1\nA=M\nD=M-D\n" + pushDToStack();
    }
    else if (theCommand == "not")
    {
      return "@SP\nA=M-1\nM=!M\n";
    }
    else if (theCommand == "neg")
    {
      return "@SP\nA=M-1\nM=-M\n";
    }
    else if (theCommand == "eq")
    {
      std::string failCase = "@SP\nA=M-1\nM=0\n@CONTINUE." + lineString + "\n0;JMP\n";
      std::string successCase = "(SUCCESS." + lineString + ")\n@SP\nA=M-1\nM=-1\n";
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JEQ\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "gt")
    {
      std::string failCase = "@SP\nA=M-1\nM=0\n@CONTINUE." + lineString + "\n0;JMP\n";
      std::string successCase = "(SUCCESS." + lineString + ")\n@SP\nA=M-1\nM=-1\n";
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JGT\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "lt")
    {
      std::string failCase = "@SP\nA=M-1\nM=0\n@CONTINUE." + lineString + "\n0;JMP\n";
      std::string successCase = "(SUCCESS." + lineString + ")\n@SP\nA=M-1\nM=-1\n";
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JLT\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "and")
    {
      return popStackToD() + "\n@SP\nA=M-1\nD=D&M\nM=D\n";
    }
    else if (theCommand == "or")
    {
      return popStackToD() + "\n@SP\nA=M-1\nD=D|M\nM=D\n";
    };
  }

  std::string popStackToD()
  {
    return "@SP\nM=M-1\nA=M\nD=M\n";
  };

  std::string pushDToStack()
  {
    return "@SP\nM=M+1\nA=M-1\nM=D\n";
  }

  std::string popSegToD(int idx, std::string segment)
  {
    return "\n@" + std::to_string(idx) + "\nD=A\n@" + memNameTable[segment] + "\nA=M\nD=D+M\n";
  }

  std::string decrementStackPointer()
  {
    return "\n@SP\nM=M-1\n";
  }

  std::string incrementStackPointer()
  {
    return "\n@SP\nM=M+1\n";
  }

  std::string setAToStack()
  {
    return "\n@SP\nA=M\n";
  }

  void close()
  {
    myfile << "\n(INFINITE_LOOP)\n@INFINITE_LOOP\n0;JMP\n";
    myfile.close();
  }

  void initTables()
  {
    // arithTable["add"] = popStackToD() + "\n@SP\nM=M-1\nA=M\nD=M+D\n" + pushDToStack();
    // arithTable["sub"] = popStackToD() + "\n@SP\nM=M-1\nA=M\nD=M-D\n" + pushDToStack();
    // arithTable["not"] = "@SP\nA=M-1\nA=A-1\nD=!M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
    // arithTable["neg"] = "@SP\nA=M-1\nM=-M\n";

    // std::string failCase = "@SP\nA=M-1\nM=0\n@CONTINUE." + std::to_string(lineIdx) + "\n0;JMP\n";
    // std::string successCase = "(SUCCESS." + std::to_string(lineIdx) + ")\n@SP\nA=M-1\nM=-1\n";

    // arithTable["eq"] = popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + std::to_string(lineIdx) + "\nD; JEQ\n" + failCase + successCase + "(CONTINUE." + std::to_string(lineIdx) + ")\n" + decrementStackPointer();
    // arithTable["gt"] = popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + std::to_string(lineIdx) + "\nD; JGT\n" + failCase + successCase + "(CONTINUE." + std::to_string(lineIdx) + ")\n" + decrementStackPointer();
    // arithTable["lt"] = popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + std::to_string(lineIdx) + "\nD; JLT\n" + failCase + successCase + "(CONTINUE." + std::to_string(lineIdx) + ")\n" + decrementStackPointer();

    // arithTable["and"] = popStackToD() + "\n@SP\nA=M-1\nD=D&M\nM=D\n";
    // arithTable["or"] = popStackToD() + "\n@SP\nA=M-1\nD=D|M\nM=D\n";

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