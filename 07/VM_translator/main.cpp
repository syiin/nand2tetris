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
  Parser myParser(*(argv + 1));
  CodeWriter myWriter(*(argv + 2));

  myParser.showTokens();

  while (myParser.hasMoreCommands())
  {
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

    myParser.advance();
  };

  myWriter.close();
};