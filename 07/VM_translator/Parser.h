#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>

class Parser
{
private:
  int line;
  std::vector<std::string> tokens;
  std::set<std::string> arithOps;

public:
  Parser(std::string inputFile)
  {
    line = 0;
    loadTokens(inputFile);
    initTables();
  }

  std::string arg1()
  {
    if (commandType() == "C_ARITHMETIC")
    {
      return removeSpaces(tokens[line]);
    }
    else
    {
      return removeSpaces(getStrBtwnStr(tokens[line], " ", " "));
    }
  }

  std::string arg2()
  {
    if (commandType() == "C_PUSH" || commandType() == "C_POP" || commandType() == "C_CALL")
    {
      int start = tokens[line].find_last_of(' ');
      int end = tokens[line].find('\0');
      return removeSpaces(tokens[line].substr(start, end));
    }
    else
    {
      return "";
    };
  }

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
  }

  void advance()
  {
    if (hasMoreCommands() == 1)
    {
      line = line + 1;
    }
  };

  std::string commandType()
  {
    if (contains(arithOps, removeSpaces(tokens[line])))
    {
      return "C_ARITHMETIC";
    }
    else if (tokens[line].substr(0, 4) == "push")
    {
      return "C_PUSH";
    }
    else if (tokens[line].substr(0, 3) == "pop")
    {
      return "C_POP";
    }
    else
    {
      return "NOT ARITH";
    }
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
        //if not a comment or white space, add to array of tokens
        tokens.push_back(line);
      }
    };
  };

  std::string removeSpaces(std::string str)
  {
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
  };

  template <typename Set>
  int contains(Set aSet, std::string aString)
  {
    if (aSet.find(aString) != aSet.end())
    {
      return 1;
    }
    else
    {
      return 0;
    }
  }

  void showTokens()
  {
    std::cout << "=======================================" << std::endl;
    std::cout << "TOKENS" << std::endl;
    std::cout << "=======================================" << std::endl;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      std::cout << *it << std::endl;
    };
    std::cout << "=======================================" << std::endl;
  };

  std::string getStrBtwnStr(const std::string &s,
                            const std::string &startDelim,
                            const std::string &stopDelim)
  {
    unsigned firstDelimPost = s.find(startDelim);
    unsigned endPosOfSecondDelim = firstDelimPost + startDelim.length();
    unsigned lastDelimPos = s.find_first_of(stopDelim, endPosOfSecondDelim);

    return s.substr(endPosOfSecondDelim,
                    lastDelimPos - endPosOfSecondDelim);
  }

  void initTables()
  {
    arithOps.insert("add");
    arithOps.insert("sub");
    arithOps.insert("neg");
    arithOps.insert("eq");
    arithOps.insert("gt");
    arithOps.insert("lt");
    arithOps.insert("and");
    arithOps.insert("or");
    arithOps.insert("not");
  }
};

#endif