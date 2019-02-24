#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "Parser.h"
#include "Code.h"

int main(int argc, char *argv[])
{
  Parser myParser(*(argv + 1));
  Code decoder;
  myParser.showTokens();
  // std::cout << myParser.getTokenIdx(7) << std::endl;

  for (int i = 0; i < myParser.numberOfTokens(); i++)
  {
    decoder.decode(myParser.getTokenIdx(i));
  };

  return 0;
}