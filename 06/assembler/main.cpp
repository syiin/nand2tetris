#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Parser.h"

int main(int argc, char *argv[])
{
  std::string fileName(*(argv + 1));
  Parser parserObj(fileName);

  // parserObj.showTokens();

  std::cout << parserObj.getTokenIdx(7) << std::endl;

  return 0;
}