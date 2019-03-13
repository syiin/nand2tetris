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
  int callCount = 0;

public:
  CodeWriter(std::string outputName)
  {
    fileName = outputName;
    myfile.open(fileName);
    initTables();
  };

  void writeArithmetic(std::string theCommand)
  {
    myfile << makeLogicString(theCommand);
  };

  void writePushPop(std::string theCommand, std::string segment, int index)
  {
    if (theCommand == "C_POP")
    {
      myfile << popSegToD(index, segment) // D = *(segment+i)
             << "@R13\nM=D\n"             // R13 = D
             << popStackToD()             // D = *(Stack)
             << "@R13\nA=M\nM=D\n";       // *(R13) = D
    }
    else if (theCommand == "C_PUSH")
    {
      if (segment == "constant")
      {
        myfile << "@" + std::to_string(index) + "\n"
               << "D=A\n"
               << pushDToStack();
      }
      else
      {
        myfile << popSegToD(index, segment)   // D = *(segment+i)
               << "A=D\nD=M\n@SP\nA=M\nM=D\n" // *(Stack) = *(D)
               << incrementStackPointer();    // Stack++
      }
    }
  };

  void writeLabel(std::string label)
  {
    myfile << "(" << label << ")\n";
  }

  void writeIf(std::string label)
  {
    myfile << popStackToD()
           << "@" + label + "\n"
           << "D; JNE\n";
  }

  void writeGoto(std::string label)
  {
    myfile << "@" + label + "\n"
           << "0; JMP\n";
  }

  void writeFunction(std::string functionName, int numVars)
  {
    myfile << "//Write: " + functionName + "\n";
    myfile << "(" << functionName << ")\n";
    for (int i = 0; i < numVars; i++)
    {
      myfile << "D=0\n"
             << pushDToStack();
    }
  }

  void writeReturn()
  {
    //TEMP VARIABLES
    std::string FRAME = "R13\n";
    std::string RETURN = "R14\n";

    myfile << "//RETURN\n";

    //FRAME = LCL
    myfile << "@LCL\n"
           << "D=M\n"
           << "@" + FRAME
           << "M=D\n";

    //RETURN = *(FRAME-5)
    myfile << "@" + FRAME
           << "D=M\n"
           << "@5\n"
           << "D=D-A\n"
           << "A=D\n"
           << "D=M\n"
           << "@" + RETURN
           << "M=D\n";

    //*ARG = pop()
    myfile << popStackToD()
           << "@ARG\n"
           << "A=M\n"
           << "M=D\n";

    //SP=ARG+1
    myfile << "@ARG\n"
           << "D=M\n"
           << "@SP\n"
           << "M=D+1\n";

    //SAVE THAT, THIS, ARG, LCL OF CALLER
    std::string addresses[4] = {"@THAT\n", "@THIS\n", "@ARG\n", "@LCL\n"};
    int offset = 1;
    for (int i = 0; i < 4; i++)
    {
      myfile << "@" + FRAME + "\n"
             << "D=M\n"
             << "@" + std::to_string(offset) + "\n"
             << "D=D-A\n"
             << "A=D\n"
             << "D=M\n" //D = *(FRAME - offset)
             << addresses[i]
             << "M=D\n"; //ADDR = D
      offset += 1;
    }

    //GOTO RETURN
    myfile << "@" + RETURN
           << "A=M\n"
           << "0;JMP\n";

    myfile << "//ENDRETURN\n";
  };

  void writeCall(std::string functionName, int numArgs)
  {
    std::string RETURN = functionName + "ret$." + std::to_string(callCount);
    callCount += 1;

    myfile << "//Call" + functionName + "\n";
    //PUSH RETURN ADDR
    myfile
        << "@" + RETURN + "\n"
        << "D=A\n"
        << pushDToStack();

    //PUSH LCL, ARG, THIS, THAT
    std::string addresses[4] = {"@LCL\n", "@ARG\n", "@THIS\n", "@THAT\n"};
    for (int i = 0; i < 4; i++)
    {
      myfile << addresses[i]
             << "D=M\n"
             << pushDToStack();
    }

    //LCL=SP
    myfile << "@SP\n"
           << "D=M\n" //D=*SP
           << "@LCL\n"
           << "M=D\n"; //LCL=D

    //ARG=SP-5
    myfile << "@" + std::to_string(numArgs + 5) + "\n"
           << "D=D-A\n" //D=D-numArgs
           << "@ARG\n"
           << "M=D\n"; //ARG=D

    //goto functionName
    myfile << "@" + functionName + "\n"
           << "0;JMP\n";

    myfile << "(" + RETURN + ")\n";
    myfile << "//EndCall" + functionName + "\n";
  }

  std::string makeLogicString(std::string theCommand)
  {
    lineIdx++;
    std::string lineString = "line" + std::to_string(lineIdx);
    std::string failCase = "@SP\nA=M-1\nM=0\n@CONTINUE." + lineString + "\n0;JMP\n";
    std::string successCase = "(SUCCESS." + lineString + ")\n@SP\nA=M-1\nM=-1\n";

    if (theCommand == "add")
    {
      return popStackToD() + "@SP\nM=M-1\nA=M\nD=M+D\n" + pushDToStack();
    }
    else if (theCommand == "sub")
    {
      return popStackToD() + "@SP\nM=M-1\nA=M\nD=M-D\n" + pushDToStack();
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
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JEQ\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "gt")
    {
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JGT\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "lt")
    {
      return popStackToD() + "@SP\nA=M-1\nD=M-D\n@SUCCESS." + lineString + "\nD; JLT\n" + failCase + successCase + "(CONTINUE." + lineString + ")\n";
    }
    else if (theCommand == "and")
    {
      return popStackToD() + "@SP\nA=M-1\nD=D&M\nM=D\n";
    }
    else if (theCommand == "or")
    {
      return popStackToD() + "@SP\nA=M-1\nD=D|M\nM=D\n";
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

  // std::string popStackToD()
  // {
  //   return decrementStackPointer() + "A=M\nD=M\n";
  // };

  // std::string pushDToStack()
  // {
  //   return "@SP\nA=M\nM=D\n" + incrementStackPointer();
  // }

  std::string popSegToD(int idx, std::string segment)
  {
    if (segment == "this")
    {
      return "\n@" + std::to_string(idx) + "\nD=A\n@" +
             memNameTable["pointer"] + "\nD=D+M\n";
    }
    else if (segment == "that")
    {
      return "\n@" + std::to_string(idx) + "\nD=A\n@" +
             memNameTable["pointer_that"] + "\nD=D+M\n";
    }
    else if (segment == "static" || segment == "pointer" || segment == "temp")
    {
      return "\n@" + std::to_string(idx) + "\nD=A\n@" +
             memNameTable[segment] + "\nD=D+A\n";
    }

    return "\n@" + std::to_string(idx) + "\nD=A\n@" +
           memNameTable[segment] + "\nD=D+M\n";
  }

  std::string popStaticPointerToD(int idx, std::string segment)
  {
    return "\n@" + std::to_string(idx) + "\nD=A\n@" +
           memNameTable[segment] + "\nD=D+A\n";
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
    memNameTable["local"] = "LCL";
    memNameTable["argument"] = "ARG";
    memNameTable["this"] = "THIS";
    memNameTable["that"] = "THAT";
    memNameTable["pointer"] = "3";
    memNameTable["pointer_that"] = "4";
    memNameTable["temp"] = "5";
    memNameTable["static"] = "16";
  }
};

#endif

/*

*/