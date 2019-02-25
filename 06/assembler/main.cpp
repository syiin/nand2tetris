#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

int main(int argc, char *argv[])
{
  std::string outputString;
  Parser myParser(*(argv + 1));
  Code decoder;
  myParser.showTokens();

  for (int i = 0; i < myParser.numberOfTokens(); i++)
  {
    if (myParser.getTokenIdx(i)[0] == '@')
    {
      outputString += "0";
    }
    else
    {
      outputString += "111";
    };

    decoder.checkAorC(myParser.getTokenIdx(i));
    outputString += "\n";
  };
  std::cout << outputString << std::endl;

  SymbolTable symbolTable;
  symbolTable.addSymbol("LOOP", 17);
  symbolTable.addSymbol("END", 33);
  std::cout << "The location of the LOOP label " << symbolTable.getSymbol("LOOP") << std::endl;
  std::cout << "The location of the END label " << symbolTable.getSymbol("END") << std::endl;

  return 0;
}