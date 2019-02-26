#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

class Parser
{
private:
  int line;
  std::vector<std::string> tokens;
  std::map<std::string, std::string> destTable;
  std::map<std::string, std::string> compTable;
  std::map<std::string, std::string> jmpTable;

public:
  Parser(std::string inputFile)
  {
    line = 0;
    loadTokens(inputFile);
  };

  int hasMoreCommands()
  {
    if (tokens.size() == line)
    {
      return 0;
    }
    else
    {
      return 1;
    }
  };

  std::string advance()
  {
    if (hasMoreCommands() == 1)
    {
      line++;
    };
  };

  std::string commandType()
  {
    if (tokens[line][0] == '@')
    {
      return "A_COMMAND";
    }
    else if (tokens[line][0] == '(')
    {
      return "L_COMMAND";
    }
    else
    {
      return "C_COMMAND";
    };
  };

  std::string symbol()
  {
    return tokens[line].substr(1, tokens.size());
  };

  std::string dest()
  {
    if (commandType().compare("C_COMMAND") == 0)
    {
      if (tokens[line].find(';') != std::string::npos)
      {
        return "null";
      }
      else
      {
        return tokens[line].substr(0, tokens[line].find('='));
      };
    };
  }

  std::string comp()
  {
    if (commandType().compare("C_COMMAND") == 0)
    {
      if (tokens[line].find(';') != std::string::npos)
      {
        int start = 0;
        int end = tokens[line].find(';');

        return tokens[line].substr(start, end);
      }
      else
      {
        int start = tokens[line].find('=') + 1;
        int end = tokens[line].find('\0');

        return removeSpaces(tokens[line].substr(start, end));
      }
    };
  };

  std::string jmp()
  {
    if (commandType().compare("C_COMMAND") == 0)
    {
      if (tokens[line].find(';') != std::string::npos)
      {

        int start = tokens[line].find(';') + 1;
        int end = tokens[line].find('\0');
        return removeSpaces(tokens[line].substr(start, end));
      }
      else
      {
        return "null";
      }
    };
  };

  std::string getCurrentCommand()
  {
    return tokens[line];
  }

  int getLine()
  {
    return line;
  }

  void setLine(int n)
  {
    line = n;
  }

  void loadTokens(std::string fileName)
  {
    std::ifstream input(fileName);
    for (std::string line; getline(input, line);)
    {
      //check for comments & white space
      if (line[0] != '/' && line[1] != '/' && line.find_first_not_of("\t\n\v\f\r") != std::string::npos)
      {
        //remove comments
        line = line.substr(0, line.find("/", 0));
        //remove white space
        line = removeSpaces(line);
        //if not a comment or white space, add to array of tokens
        tokens.push_back(line);
      }
    };
  };

  void initTables()
  {
    destTable["null"] = "000";
    destTable["M"] = "001";
    destTable["D"] = "010";
    destTable["MD"] = "011";
    destTable["A"] = "100";
    destTable["AM"] = "101";
    destTable["AD"] = "110";
    destTable["AMD"] = "111";

    compTable["0"] = "0101010";
    compTable["1"] = "0111111";
    compTable["-1"] = "0111010";
    compTable["D"] = "0001100";
    compTable["A"] = "0110000";

    compTable["!D"] = "0001101";
    compTable["!A"] = "0110001";
    compTable["-D"] = "0001111";
    compTable["-A"] = "0111111";
    compTable["D+1"] = "0011111";
    compTable["A+1"] = "0110111";

    compTable["D-1"] = "0001110";
    compTable["A-1"] = "0110010";
    compTable["D+A"] = "0000010";
    compTable["D-A"] = "0010011";
    compTable["A-D"] = "0000111";

    compTable["D&A"] = "0000000";
    compTable["D|A"] = "0010101";
    compTable["M"] = "1110000";
    compTable["!M"] = "1110001";
    compTable["-M"] = "1110011";
    compTable["M+1"] = "1110010";

    compTable["D+M"] = "1000010";
    compTable["D-M"] = "1010011";
    compTable["M-D"] = "1000111";
    compTable["D&M"] = "1000000";
    compTable["D|M"] = "1010101";

    jmpTable["null"] = "000";
    jmpTable["JGT"] = "001";
    jmpTable["JEQ"] = "010";
    jmpTable["JGE"] = "011";
    jmpTable["JLT"] = "100";
    jmpTable["JNE"] = "101";
    jmpTable["JLE"] = "110";
    jmpTable["JMP"] = "111";
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
  };

  std::string removeSpaces(std::string str)
  {
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    // \t\n\v\f\r
    return str;
  };
};
#endif
/*

*/