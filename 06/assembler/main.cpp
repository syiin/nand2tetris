#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>

#include "Parser.h"
#include "Code.h"
#include "SymbolTable.h"

bool is_number(const std::string &str);

int main(int argc, char *argv[])
{
  std::string outputString = "";
  Parser myParser(*(argv + 1));
  Code myDecoder;
  SymbolTable mySymbolTable;
  myParser.showTokens();

  //FIRST PASS
  std::cout << "LABELS" << std::endl;
  std::cout << "=======================================" << std::endl;
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "L_COMMAND")
    {
      std::string currentCommand = myParser.getCurrentCommand();
      std::cout << "COMMAND: \t";
      std::cout << currentCommand << std::endl;
      int start = currentCommand.find('(') + 1; // remove the () parantheses
      int end = currentCommand.find(')') - 1;
      currentCommand = currentCommand.substr(start, end);
      mySymbolTable.addLabel(currentCommand, myParser.getLine()); //add the label at the current line
      myParser.removeLCommand();
    };
    myParser.advance();
  };
  std::cout << "=======================================" << std::endl;

  //SECOND PASS
  myParser.setLine(0); //reset the parser to the top
  while (myParser.hasMoreCommands())
  {
    if (myParser.commandType() == "C_COMMAND") //handle C commands
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
    else if (myParser.commandType() == "A_COMMAND") //handle A commands
    {
      std::cout << "Line: \t" << myParser.getLine() << std::endl;

      std::string currentCommand = myParser.getCurrentCommand();
      int end = currentCommand.find('\0');
      currentCommand = currentCommand.substr(1, end);
      std::cout << "Var: \t" << currentCommand << std::endl;

      //if @xxx is a symbol
      int x;
      if (!is_number(currentCommand))
      {
        mySymbolTable.addVariable(currentCommand); //try to add the variable if first encounter
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

  std::cout << "SYMBOL TABLE" << std::endl;
  std::cout << "=======================================" << std::endl;
  mySymbolTable.showSymbols();

  return 0;
}

bool is_number(const std::string &str)
{
  return str.find_first_not_of("0123456789") == std::string::npos;
};