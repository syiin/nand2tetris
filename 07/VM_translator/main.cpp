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
  int i = 1;
  std::cout << "===============================" << std::endl;

  while (myParser.hasMoreCommands())
  {
    std::cout << "Line: \t" << i << std::endl;
    std::cout << "C Typ: \t" << myParser.commandType() << std::endl;
    std::cout << "Arg 1: \t" << myParser.arg1() << std::endl;
    std::cout << "Arg 2: \t" << myParser.arg2() << std::endl;
    if (myParser.commandType() == "C_ARITHMETIC")
    {
      std::cout << "writeArith: \n"
                << myWriter.writeArithmetic(myParser.arg1()) << std::endl;
    }
    else if (myParser.commandType() == "C_PUSH" || myParser.commandType() == "C_POP")
    {
      std::cout << "writePushPop: \n"
                << myWriter.writePushPop(myParser.commandType(), myParser.arg1(), std::stoi(myParser.arg2())) << std::endl;
    };
    std::cout << "===============================" << std::endl;

    i++;
    myParser.advance();
  };
}