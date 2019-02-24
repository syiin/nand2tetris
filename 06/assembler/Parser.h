#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>

class Parser
{
private:
  std::vector<std::string> tokens;

public:
  //constructor
  Parser(std::string inputFile)
  {
    loadTokens(inputFile);
  };

  void loadTokens(std::string fileName)
  {
    std::ifstream input(fileName);
    for (std::string line; getline(input, line);)
    {
      //check for comments & white space
      if (line[0] != '/' && line[1] != '/' && line.find_first_not_of("\t\n\v\f\r") != std::string::npos)
      {
        //if not a comment or white space, add to array of tokens
        tokens.push_back(line);
      }
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

  int numberOfTokens()
  {
    return tokens.size();
  }
};

#endif