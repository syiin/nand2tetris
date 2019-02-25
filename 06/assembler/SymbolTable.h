#ifndef SYMT_H
#define SYMT_H

#include <string>
#include <algorithm>
#include <map>

class SymbolTable
{

private:
  std::map<std::string, int> symbols;

public:
  void addSymbol(std::string labelValue, int memLocation)
  {
    if (symbols[labelValue] == 0)
    {
      symbols[labelValue] = memLocation;
    };
  };

  int getSymbol(std::string labelValue)
  {
    return symbols[labelValue];
  }
};

#endif