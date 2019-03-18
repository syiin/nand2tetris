#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <experimental/filesystem>

using namespace std;

class Tokenizer
{

private:
  int line;
  vector<char> charTokens;
  vector<string> stringTokens;

  vector<string> TOKEN_TYPES = {"KEYWORD", "SYMBOL", "IDENTIFIER", "INT_CONST", "STRING_CONST"};
  vector<string> TOKEN_KEYWORDS = {"CLASS", "METHOD", "FUNCTION", "CONSTRUCTOR", "INT",
                                   "BOOLEAN", "CHAR", "VOID", "VAR", "STATIC", "FIELD",
                                   "LET", "DO", "IF", "ELSE", "WHILE", "RETURN", "TRUE",
                                   "FALSE", "NULL", "THIS"};
  vector<string> SYMBOLS = {"(", ")", "[", "]", "{", "}", ",", ";", "=", ".", "+", "-", "*",
                            "/", "&", "|", "~", "<", ">"};

public:
  Tokenizer(string fileName)
  {
    loadTokens(fileName);
  }

  void loadTokens(string fileName)
  {
    resetTokenizer();
    initStringTokens(fileName);
  }

  void initStringTokens(string fileName)
  {
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      istringstream iss(line.substr(0, line.find("/", 0)));
      for (string token; getline(iss, token, ' ');)
        if (checkNotCommentOrSpace(token))
        {
          addLineToTokens(token);
        }
    };
  }
  void addLineToTokens(string line)
  {
    stringTokens.push_back(line);
  }

  void resetTokenizer()
  {
    charTokens.clear();
    line = 0;
  }

  //CHECKERS
  bool checkForSemiColon(string line)
  {
    return (line.find(';') != string::npos);
  }

  bool checkForBrackets(string line)
  {
    return (line.find_first_not_of("()[]") != string::npos);
  }

  bool checkNotCommentOrSpace(string line)
  {
    return (line[0] != '/' && line[1] != '/' && line.find_first_not_of("\t\n\v\f\r") != string::npos);
  }

  bool isKeyword(string line)
  {
    if (std::find(TOKEN_KEYWORDS.begin(), TOKEN_KEYWORDS.end(), line) != TOKEN_KEYWORDS.end())
      return true;
    else
      return false;
  }

  bool isSymbol(string c)
  {
    if (std::find(SYMBOLS.begin(), SYMBOLS.end(), c) != SYMBOLS.end())
      return true;
    else
      return false;
  }

  // bool containsBrackets(string line)
  // {
  //   if (std::find(BRACKETS.begin(), BRACKETS.end(), line) != BRACKETS.end())
  //     return true;
  //   else
  //     return false;
  // }

  void printTokens()
  {
    cout << "=======================================" << endl;
    cout << "TOKENS" << endl;
    cout << "=======================================" << endl;
    for (vector<string>::iterator it = stringTokens.begin(); it != stringTokens.end(); ++it)
    {
      cout << *it << endl;
    };
    cout << "=======================================" << endl;
  }
};

#endif

/*

ARCHIVE:

string getStringInCharToken()
  {
    string word = "";
    for (vector<char>::iterator it = charTokens.begin(); it != charTokens.end(); ++it)
    {
      if (!isSymbol(*it))
      {
        word = word + (*it);
      }
      else
      {
        stringTokens.push_back(word);
        word = "";
      }

      if (isKeyword(word))
      {
        stringTokens.push_back(word);
        word = "";
      }
    };
  }

  void initCharTokens(string fileName)
  {
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      if (checkNotCommentOrSpace(line))
      {
        line = line.substr(0, line.find("/", 0));
        addCharToTokens(line);
      }
    };
  }

  void addCharToTokens(string line)
  {
    for (const char &c : line)
    {
      addCharWithoutSpaces(c);
    }
  };

  void addCharWithoutSpaces(char c)
  {
    if (c != ' ' && c != '\r')
    {
      charTokens.push_back(c);
    }
  }

void initStringTokens(string fileName)
  {
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      istringstream iss(line.substr(0, line.find("/", 0)));
      for (string token; getline(iss, token, ' ');)
        if (checkNotCommentOrSpace(token))
        {
          if (checkForSemiColon(token))
          {
            addSCToTokens(token);
          }
          else if (checkForBrackets(token))
          {
          }
          else
          {
            addLineToTokens(token);
          }
        }
    };
  }

  void addBracketToTokens(string line)
  {
    if (line[0] == '(')
    {
      int start = line.find('(');
      int end = line.find(')');
      addLineToTokens("(");
      addLineToTokens(line.substr(start, end));
      addLineToTokens(")");
    }
    else if (line[0] == '[')
    {
      int start = line.find('[');
      int end = line.find(']');
      addLineToTokens("[");
      addLineToTokens(line.substr(start, end));
      addLineToTokens("]");
    }
  }

    void addLineToTokens(string line)
  {
    stringTokens.push_back(line);
  }

*/