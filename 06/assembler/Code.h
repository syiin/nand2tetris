#ifndef CODE_H
#define CODE_H

#include <string>
#include <iostream>

class Code
{
public:
  std::string comp(std::string compString)
  {
    if (compString == "M+1")
    {
      return "1110111";
    };
  };

  std::string dest(std::string destString)
  {
    if (destString == "D")
    {
      return "010";
    };
  };

  std::string jmp(std::string jmpString)
  {
    if (jmpString == "JGT")
    {
      return "001";
    };
  };

  void checkAorC(std::string inputString)
  {
    if (inputString[0] == '@')
    {
      std::cout << "It's an A instruction" << std::endl;
    }
    else
    {
      std::cout << "It's a C instruction" << std::endl;
    };
  };
};

#endif