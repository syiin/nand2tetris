#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>

#include "Parser.h"

int main(int argc, char *argv[])
{
  std::string outputString = "";
  Parser myParser(*(argv + 1));
  myParser.showTokens();
  int i = 1;
  std::cout << "===============================" << std::endl;
  while (myParser.hasMoreCommands())
  {
    std::cout << "Line: \t" << i << std::endl;
    std::cout << "C Typ: \t" << myParser.commandType() << std::endl;
    std::cout << "Arg 1: \t" << myParser.arg1() << std::endl;
    std::cout << "Arg 2: \t" << myParser.arg2() << std::endl;
    std::cout << "===============================" << std::endl;

    i++;
    myParser.advance();
  }
}