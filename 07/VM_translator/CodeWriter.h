#ifndef CODE_H
#define CODE_H

#include <fstream>
#include <map>
#include <string>

class CodeWriter
{

private:
  std::map<std::string, std::string> arithTable;
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
      myfile << "\n@" + std::to_string(index) + "\nD=A\n@" + segment + "\nD=D+A\n@temp\nM=D\n@SP\nM=M-1\nD=M\n@temp\nA=M\nM=D\n";
      return "@" + std::to_string(index) + "\nD=A\n@" + segment + "\nD=D+A\n@temp\nM=D\n@SP\nM=M-1\nD=M\n@temp\nA=M\nM=D\n";
    }
    else if (theCommand == "C_PUSH")
    {
      if (segment == "constant")
      {
        myfile << "\n@" + std::to_string(index) + "\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n";
        return "@" + std::to_string(index) + "\nD=A\n@SP\nM=M+1\nA=M-1\nM=D\n";
      }
      else
      {
        myfile << "\n@" + std::to_string(index) + "\nD=A\n@" + segment + "\nD=D+A\nA=D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
        return "@" + std::to_string(index) + "D=A\n@" + segment + "D=D+A\nA=D\nD=M\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
      }
    }
  };

  void close()
  {
    myfile.close();
  }

  void initTables()
  {
    arithTable["add"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D+M\n@SP\nM=M-1\nA=M-1\nM=D";
    arithTable["sub"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D-M\n@SP\nM=M-1\nA=M-1\nM=D";
    arithTable["neg"] = "@SP\nA=M-1\nM=-M";
    arithTable["eq"] = "@SP\nA=M-1\nD=M\n@0\nD=D&A\n@SP\nA=M-1\nM=D";
    arithTable["gt"] = "";
    arithTable["lt"] = "";
    arithTable["and"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D|M\n@SP\nM=M-1\nA=M-1\nM=D";
    arithTable["or"] = "@SP\nA=M-1\nD=M\nA=A-1\nD=D&M\n@SP\nM=M-1\nA=M-1\nM=D";
    arithTable["not"] = "@SP\nA=M-1\nA=A-1\nD=!M\n@SP\nA=M\nM=D\n@SP\nM=M+1";
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