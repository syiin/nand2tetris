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

  vector<string> TOKEN_KEYWORDS = {"class", "method", "function", "constructor", "int",
                                   "boolean", "char", "void", "var", "static", "field",
                                   "let", "do", "if", "else", "while", "return", "true",
                                   "false", "null", "this"};

  vector<string> SYMBOLS = {"(", ")", "[", "]", "{", "}", ",", ";", "=", ".", "+", "-", "*",
                            "/", "&", "|", "~", "<", ">"};

public:
  bool hasMoreTokens()
  {
    return (tokenCount <= stringTokens.size());
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
      return "keyword";
    }
    else if (isSymbol(token))
    {
      return "symbol";
    }
    else if (isString(token))
    {
      return "stringConstant";
    }
    else if (isNumber(token))
    {
      return "integerConstant";
    }
    else
    {
      return "identifier";
    }
  }

  string keyWord()
  {
    if (tokenType() == "keyword")
    {
      return stringTokens[tokenCount];
    }
  }

  string symbol()
  {
    if (tokenType() == "symbol")
    {
      return sanitiseSymbolCharacter(stringTokens[tokenCount]);
    }
  }

  string intVal()
  {
    if (tokenType() == "integerConstant")
    {
      return stringTokens[tokenCount];
    }
  }

  string stringVal()
  {
    if (tokenType() == "stringConstant")
    {
      int start = stringTokens[tokenCount].find('"') + 1;
      int end = stringTokens[tokenCount].find_last_of('"') - 1;
      return stringTokens[tokenCount].substr(start, end);
    }
  }

  string identifier()
  {
    if (tokenType() == "identifier")
    {
      return stringTokens[tokenCount];
    }
  }

  string lookAheadString()
  {
    return stringTokens[tokenCount];
  }

  string lookBehindString()
  {
    return stringTokens[tokenCount - 2];
  }

  string lookAheadType()
  {
    return tokenType();
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
    removeTokenSpaces();
  }

  void populateTokens(string fileName)
  {
    ifstream input(fileName);
    for (string line; getline(input, line);)
    {
      line = removeCommentsFromString(line);
      addLineToTokens(line);
    };
  }

  void splitBySymbols()
  {
    for (vector<string>::iterator it = SYMBOLS.begin(); it != SYMBOLS.end(); ++it)
    {
      stringTokens = splitVecByChar(stringTokens, (*it)[0]);
    };
  }

  void removeTokenSpaces()
  {
    stringTokens = splitStringsAndRemoveInVec(stringTokens, ' ');
    // stringTokens = splitStringsAndRemoveInVec(stringTokens, '\r');
    // stringTokens = splitStringsAndRemoveInVec(stringTokens, '\t');
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

  //VECTOR MANIPULATION FUNCTIONS
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

  vector<string> wrapStringInVector(string line)
  {
    stringstream inputString(line);
    string segment;
    vector<string> outputVector;
    while (getline(inputString, segment))
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
      if (!isString(removeSpaces(*it)))
      {
        tempVec = splitStringAndRemoveByChar(*it, c);
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
      else
      {
        tempVec = wrapStringInVector(removeSpaces(*it));
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
    };
    return outputVec;
  }

  //STRING MANIPULATION

  string removeSpaces(std::string str)
  {
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
  };

  string makeUpper(string inputString)
  {
    string outputString = "";
    for (const char &c : inputString)
    {
      outputString = outputString + to_string(toupper(c));
    }
    return outputString;
  }

  string sanitiseSymbolCharacter(string character)
  {
    if (character == "<")
      return "&lt;";
    else
      return character;
  }

  string removeCommentsFromString(string line)
  {
    string tokenInput = line.substr(0, line.find("//", 0));
    tokenInput = tokenInput.substr(0, line.find("/**", 0));
    return tokenInput;
  }

  //CHECKERS
  bool checkNotCommentOrSpace(string line)
  {
    return (line.find_first_not_of("\t\n\v\f\r") != string::npos);
  }

  bool isKeyword(const string &line)
  {
    if (find(TOKEN_KEYWORDS.begin(), TOKEN_KEYWORDS.end(), line) != TOKEN_KEYWORDS.end())
      return true;
    else
      return false;
  }

  bool isSymbol(const string &line)
  {
    if (find(SYMBOLS.begin(), SYMBOLS.end(), line) != SYMBOLS.end())
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
