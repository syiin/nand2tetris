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
  int tokenCount;
  vector<char> charTokens;
  vector<string> stringTokens;

  vector<string> TOKEN_KEYWORDS = {"CLASS", "METHOD", "FUNCTION", "CONSTRUCTOR", "INT",
                                   "BOOLEAN", "CHAR", "VOID", "VAR", "STATIC", "FIELD",
                                   "LET", "DO", "IF", "ELSE", "WHILE", "RETURN", "TRUE",
                                   "FALSE", "NULL", "THIS"};
  vector<string> SYMBOLS = {"(", ")", "[", "]", "{", "}", ",", ";", "=", ".", "+", "-", "*",
                            "/", "&", "|", "~", "<", ">"};

public:
  bool hasMoreTokens()
  {
    return (tokenCount == stringTokens.size());
  }

  void advance()
  {
    if (hasMoreTokens())
    {
      tokenCount++;
    }
  }

  string tokenType()
  {
    const string token = stringTokens[tokenCount];
    if (isKeyword(token))
    {
      return "KEYWORD";
    }
    else if (isSymbol(token))
    {
      return "SYMBOL";
    }
    else if (isString(token))
    {
      return "STRING_CONST";
    }
    else if (isNumber(token))
    {
      return "INT_CONST";
    }
    else
    {
      return "IDENTIFIER";
    }
  }

  string keyWord()
  {
    if (tokenType() == "KEYWORD")
    {
      return makeUpper(stringTokens[tokenCount]);
    }
  }

  string symbol()
  {
    if (tokenType() == "SYMBOL")
    {
      return stringTokens[tokenCount];
    }
  }

  string intVal()
  {
    if (tokenType() == "INT_CONST")
    {
      return stringTokens[tokenCount];
    }
  }

  string stringVal()
  {
    if (tokenType() == "STRING_CONST")
    {
      return stringTokens[tokenCount];
    }
  }

  //TOKEN MANIPULATION
  void loadTokens(string fileName)
  {
    resetTokenizer();
    initStringTokens(fileName);
  }

  void initStringTokens(string fileName)
  {
    populateTokens(fileName);
    splitBySymbols();
    stringTokens = splitStringsAndRemoveInVec(stringTokens, ' ');
  }

  void populateTokens(string fileName)
  {
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      string tokenInput = line.substr(0, line.find("/", 0));
      stringTokens.push_back(tokenInput);
    };
  }

  void splitBySymbols()
  {
    for (vector<string>::iterator it = SYMBOLS.begin(); it != SYMBOLS.end(); ++it)
    {
      stringTokens = splitVecByChar(stringTokens, (*it)[0]);
    };
  }

  void addLineToTokens(string line)
  {
    stringTokens.push_back(line);
  }

  void resetTokenizer()
  {
    charTokens.clear();
    tokenCount = 0;
  }

  //STRING & VECTOR MANIPULATION FUNCTIONS
  vector<string> splitVecByChar(vector<string> inputVec, char c)
  {
    vector<string> tempVec;
    vector<string> outputVec;
    for (vector<string>::iterator it = inputVec.begin(); it != inputVec.end(); ++it)
    {
      tempVec = splitStringByChar(*it, c);
      outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
    };
    return outputVec;
  }

  vector<string> splitStringByChar(string line, char splitChar)
  {
    vector<string> outputVec;
    string tempString = "";

    for (const char &c : line)
    {
      if (c == splitChar)
      {
        outputVec.push_back(tempString);
        outputVec.push_back(string(1, c));
        tempString = "";
      }
      else
      {
        tempString = tempString + c;
      }
    }
    outputVec.push_back(tempString);
    return outputVec;
  }

  vector<string> splitStringAndRemoveByChar(string line, char c)
  {
    stringstream inputString(line);
    string segment;
    vector<string> outputVector;
    while (getline(inputString, segment, c))
    {
      if (checkNotCommentOrSpace(segment))
      {
        outputVector.push_back(segment);
      }
    }
    return outputVector;
  }

  vector<string> splitStringsAndRemoveInVec(vector<string> inputVec, char c)
  {
    vector<string> tempVec;
    vector<string> outputVec;
    for (vector<string>::iterator it = inputVec.begin(); it != inputVec.end(); ++it)
    {
      tempVec = splitStringAndRemoveByChar(*it, c);
      outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
    };
    return outputVec;
  }

  string makeUpper(string inputString)
  {
    string outputString = "";
    for (const char &c : inputString)
    {
      outputString = outputString + to_string(toupper(c));
    }
    return outputString;
  }

  //CHECKERS
  bool checkNotCommentOrSpace(string line)
  {
    return (line[0] != '/' && line[1] != '/' && line.find_first_not_of("\t\n\v\f\r") != string::npos);
  }

  bool isKeyword(const string &line)
  {
    if (std::find(TOKEN_KEYWORDS.begin(), TOKEN_KEYWORDS.end(), line) != TOKEN_KEYWORDS.end())
      return true;
    else
      return false;
  }

  bool isSymbol(const string &line)
  {
    if (std::find(SYMBOLS.begin(), SYMBOLS.end(), line) != SYMBOLS.end())
      return true;
    else
      return false;
  }

  bool isString(const string &line)
  {
    if (line[0] == '"')
      return true;
    else
      return false;
  }

  bool isNumber(const string &line)
  {
    return all_of(line.begin(), line.end(), ::isdigit);
  }

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