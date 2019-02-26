#ifndef SYMT_H
#define SYMT_H

#include <string>
#include <algorithm>
#include <map>

class SymbolTable
{

private:
  std::map<std::string, int> symbols;
  int n = 16;

public:
  SymbolTable()
  {
    n = 16;
    initTables();
  };

  int contains(std::string labelValue)
  {

    if (symbols.count(labelValue))
    {
      return 1;
    }
    else
    {
      return 0;
    };
  }

  void addLabel(std::string labelKey, int memLocation)
  {
    symbols[labelKey] = memLocation;
  };

  void addVariable(std::string varKey)
  {
    if (!contains(varKey))
    {
      symbols[varKey] = n;
      n++;
    };
  };

  int getSymbol(std::string labelValue)
  {
    if (contains(labelValue))
    {
      return symbols[labelValue];
    };
  }

  void showSymbols()
  {
    for (std::map<std::string, int>::iterator it = symbols.begin(); it != symbols.end(); ++it)
    {
      std::cout << it->first << "\t" << it->second << std::endl;
    };
  };

  void initTables()
  {
    symbols["R0"] = 0;
    symbols["R1"] = 1;
    symbols["R2"] = 2;
    symbols["R3"] = 3;
    symbols["R4"] = 4;
    symbols["R5"] = 5;
    symbols["R6"] = 6;
    symbols["R7"] = 7;
    symbols["R8"] = 8;
    symbols["R9"] = 9;
    symbols["R10"] = 10;
    symbols["R11"] = 11;
    symbols["R12"] = 12;
    symbols["R13"] = 13;
    symbols["R14"] = 14;
    symbols["R15"] = 15;

    symbols["SP"] = 0;
    symbols["LCL"] = 1;
    symbols["ARG"] = 2;
    symbols["THIS"] = 3;
    symbols["THAT"] = 4;
    symbols["LCL"] = 1;

    symbols["SCREEN"] = 16384;
    symbols["KBD"] = 24576;
  };
};

#endif