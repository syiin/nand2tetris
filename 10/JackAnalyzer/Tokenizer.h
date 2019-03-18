#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>
#include <experimental/filesystem>

using namespace std;

class Tokenizer
{

private:
  int line;
  vector<string> tokens;

  vector<string> TOKEN_TYPES = [ "KEYWORD", "SYMBOL", "IDENTIFIER", "INT_CONST", "STRING_CONST" ];
  vector<string> TOKEN_KEYWORDS = [ "CLASS", "METHOD", "FUNCTION", "CONSTRUCTOR", "INT",
                                    "BOOLEAN", "CHAR", "VOID", "VAR", "STATIC", "FIELD",
                                    "LET", "DO", "IF", "ELSE", "WHILE", "RETURN", "TRUE",
                                    "FALSE", "NULL", "THIS" ];
  vector<string> SYMBOLS = [ '(', ')', '[', ']', '{', '}', ',', ';', '=', '.', '+', '-', '*',
                             '/', '&', '|', '~', '<', '>' ];
  vector<string> WHITE_SPACE = [ ' ', '\n', '\t' ];

public:
  Tokenizer(string fileName)
  {
    resetTokenizer();
    loadTokens(fileName);
  }

  void loadTokens(string fileName)
  {
    resetTokenizer();
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      if (checkNotCommentOrSpace(line))
      {
        line = line.substr(0, line.find("/", 0));
        addCharacterToTokens(line)
      }
    };
  }

  bool checkNotCommentOrSpace(string line)
  {
    return (line[0] != '/' && line[1] != '/' && line.find_first_not_of("\t\n\v\f\r") != string::npos)
  }

  void addCharacterToTokens(string line)
  {
    for (const char &c : line)
    {
      tokens.push_back(c);
    }
  }

  void resetTokenizer()
  {
    tokens.clear();
    line = 0;
  }

  void printTokens()
  {
    cout << "=======================================" << endl;
    cout << "TOKENS" << endl;
    cout << "=======================================" << endl;
    for (vector<string>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
      cout << *it << endl;
    };
    cout << "=======================================" << endl;
  }
};

#endif