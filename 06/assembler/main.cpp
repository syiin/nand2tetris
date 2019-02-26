#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>

#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

bool is_number(const std::string &s);

int main(int argc, char *argv[])
{
  std::string outputString = "";
  Parser myParser(*(argv + 1));
  Code myDecoder;
  SymbolTable mySymbolTable;
  myParser.showTokens();

  //FIRST PASS
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "L_COMMAND")
    {
      std::string currentCommand = myParser.getCurrentCommand();
      int start = currentCommand.find('(');
      int end = currentCommand.find(')');
      currentCommand = currentCommand.substr(start, end);
      mySymbolTable.addSymbol(currentCommand);
    }
    else if (myParser.commandType() == "A_COMMAND")
    {
      std::string currentCommand = myParser.getCurrentCommand();
      int start = currentCommand.find('@');
      int end = currentCommand.find('\0');
      currentCommand = currentCommand.substr(start, end);
      mySymbolTable.addSymbol(currentCommand);
    };
    myParser.advance();
  };

  //SECOND PASS
  myParser.setLine(0); //reset the parser to the top
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "C_COMMAND")
    {
      std::cout << "Line: \t" << myParser.getLine() << std::endl;

      std::string destBin = myDecoder.dest(myParser.dest());
      std::string compBin = myDecoder.comp(myParser.comp());
      std::string jmpBin = myDecoder.jmp(myParser.jmp());

      std::cout << "D: \t" << myParser.dest() << std::endl;
      std::cout << "C: \t" << myParser.comp() << std::endl;
      std::cout << "J: \t" << myDecoder.jmp(myParser.jmp()) << std::endl;

      outputString = outputString + "111" + compBin + destBin + jmpBin + "\n";
    }
    else if (myParser.commandType() == "A_COMMAND")
    {
      std::cout << "Line: \t" << myParser.getLine() << std::endl;

      std::string currentCommand = myParser.getCurrentCommand();
      int end = currentCommand.find('\0');
      currentCommand = currentCommand.substr(1, end);

      //if @xxx is a symbol
      int x;
      if (!is_number(currentCommand))
      {
        int end = currentCommand.find('\0');
        currentCommand = currentCommand.substr(1, end);
        x = mySymbolTable.getSymbol(currentCommand);
      }
      else
      {
        //if @xxx is a number
        x = stoi(currentCommand);
      };
      std::string binString = std::bitset<15>(x).to_string();
      std::cout << "A:\t"
                << std::bitset<15>(x) << "\t"
                << std::endl;
      outputString = outputString + "0" + binString + "\n";
    };
    std::cout << "=======================================" << std::endl;
    myParser.advance();
  };

  std::cout << outputString << std::endl;

  std::ofstream myfile;
  std::string outputFile = *(argv + 2);
  myfile.open(outputFile);
  myfile << outputString;
  myfile.close();

  return 0;
}

bool is_number(const std::string &s)
{
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isdigit(*it))
    ++it;
  return !s.empty() && it == s.end();
}
