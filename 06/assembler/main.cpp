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
void checkBinString(std::string outputString);

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

      int start = currentCommand.find('(') + 1; // remove the () parantheses
      int end = currentCommand.find(')') - 1;
      currentCommand = currentCommand.substr(start, end);
      mySymbolTable.addLabel(currentCommand, myParser.getLine()); //add the label at the current line
      myParser.removeLCommand();                                  //remove the L command from the token list
    };
    myParser.advance();
  };

  mySymbolTable.showSymbols(); //Check symbol table after labels have been added

  //SECOND PASS
  myParser.setLine(0); //reset the parser to the top
  while (myParser.hasMoreCommands())
  {
    std::cout << "Line: \t" << myParser.getLine() << std::endl;
    if (myParser.commandType() == "C_COMMAND") //handle C commands
    {
      std::string destBin = myDecoder.dest(myParser.dest());
      std::string compBin = myDecoder.comp(myParser.comp());
      std::string jmpBin = myDecoder.jmp(myParser.jmp()); //get the binary values for each component of the code string

      std::cout << "D: \t" << myParser.dest() << std::endl;
      std::cout << "C: \t" << myParser.comp() << std::endl;
      std::cout << "J: \t" << myDecoder.jmp(myParser.jmp()) << std::endl;

      outputString = outputString + "111" + compBin + destBin + jmpBin + "\n"; //concatenate the c instruction binary
    }
    else if (myParser.commandType() == "A_COMMAND") //handle A commands
    {

      std::string currentCommand = myParser.getCurrentCommand();
      int end = currentCommand.find('\0');
      currentCommand = currentCommand.substr(1, end);

      std::cout << "Var: \t" << currentCommand << std::endl;

      int x;
      if (!is_number(currentCommand)) //if @xxx is a symbol
      {
        mySymbolTable.addVariable(currentCommand); //try to add the variable if first encounter
        x = mySymbolTable.getSymbol(currentCommand);
      }
      else //if @xxx is a number
      {
        x = stoi(currentCommand);
      };

      std::string binString = std::bitset<15>(x).to_string();
      std::cout << "A:\t"
                << std::bitset<15>(x) << "\t"
                << std::endl;
      outputString = outputString + "0" + binString + "\n"; //concatenate the a instruction binary
    };
    std::cout << "=======================================" << std::endl;
    myParser.advance(); //take one step
  };

  //Check symbol table after variables have been added
  mySymbolTable.showSymbols();

  //Check string and save file
  checkBinString(outputString);
  std::ofstream myfile;
  std::string outputFile = *(argv + 2);
  myfile.open(outputFile);
  myfile << outputString;
  myfile.close();

  return 0;
}

bool is_number(const std::string &str)
{
  return str.find_first_not_of("0123456789") == std::string::npos;
};

void checkBinString(std::string outputString)
{
  std::cout << "=======================================" << std::endl;
  std::cout << "BINARY OUTPUT" << std::endl;
  std::cout << "=======================================" << std::endl;
  std::cout << outputString << std::endl;
};
