#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>

#include "Parser.h"
#include "CodeWriter.h"

int main(int argc, char *argv[])
{
  std::string outputString = "";
  Parser myParser(*(argv + 1));
  CodeWriter myWriter(*(argv + 2));

  myParser.showTokens();

  while (myParser.hasMoreCommands())
  {
    std::cout << "C_Typ: \t" << myParser.commandType() << std::endl;
    std::cout << "Arg1: \t" << myParser.arg1() << std::endl;
    std::cout << "Arg2: \t" << myParser.arg2() << std::endl;

    if (myParser.commandType() == "C_ARITHMETIC")
    {
      myWriter.writeArithmetic(myParser.arg1());
    }
    else if (myParser.commandType() == "C_PUSH" || myParser.commandType() == "C_POP")
    {
      myWriter.writePushPop(myParser.commandType(),
                            myParser.arg1(),
                            std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_LABEL")
    {
      myWriter.writeLabel(myParser.arg1());
    }
    else if (myParser.commandType() == "C_IF")
    {
      myWriter.writeIf(myParser.arg1());
    }
    else if (myParser.commandType() == "C_GOTO")
    {
      myWriter.writeGoto(myParser.arg1());
    }
    else if (myParser.commandType() == "C_FUNCTION")
    {
      myWriter.writeFunction(myParser.arg1(), std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_RETURN")
    {
      myWriter.writeReturn();
    };
    myParser.advance();
  };
}