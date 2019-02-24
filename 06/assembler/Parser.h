#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Parser
{
public:
  std::string fileName;
  std::vector<std::string> tokens;

  Parser(std::string inputFile)
  {
    std::cout << inputFile << std::endl;
    fileName = inputFile;

    loadTokens();
  };

  void loadTokens()
  {
    std::ifstream input(fileName);
    for (std::string line; getline(input, line);)
    {
      tokens.push_back(line);
    };
  };

  void showTokens()
  {
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      std::cout << *it << std::endl;
    };
  };

  std::string getTokenIdx(int idx)
  {
    return tokens[idx];
  };
};

#endif