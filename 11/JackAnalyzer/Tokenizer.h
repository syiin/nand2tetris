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
      return stringTokens[tokenCount];
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
      return stringTokens[tokenCount];
    }
  }

  string identifier()
  {
    if (tokenType() == "identifier")
    {
      return stringTokens[tokenCount];
    }
  }

  string lookAheadType()
  {
    return tokenType();
  }

  string lookAheadString()
  {
    return stringTokens[tokenCount];
  }

  string lookBehindString()
  {
    return stringTokens[tokenCount - 2];
  }

  string lookBehindType()
  {
    const string token = stringTokens[tokenCount - 2];

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

  void pointerBack()
  {
    if (tokenCount > 0)
      tokenCount--;
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
    processStringVec();
    splitBySymbols();
    removeTokenSpaces();
  }

  void populateTokens(string fileName)
  {
    ifstream input(fileName);
    string lastLine = " ";
    bool inComment = false;
    for (string line; getline(input, line);)
    {
      if (line.substr(0, 4) == "/**" || removeSpaces(line)[0] == '*' || removeSpaces(line) == "*/")
        inComment = true;
      else
        inComment = false;

      if (!inComment)
      {
        line = removeCommentsFromString(line);
        addLineToTokens(line);
      }
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
    stringTokens = splitStringsAndRemoveInVec(stringTokens, '\r');
    stringTokens = splitStringsAndRemoveInVec(stringTokens, '\t');
  }

  void addLineToTokens(string line)
  {

    stringTokens.push_back(line);
  }

  void resetTokenizer()
  {
    stringTokens.clear();
    tokenCount = 0;
  }

  //VECTOR MANIPULATION FUNCTIONS

  void removeEmptyItems()
  {
    for (int i = 0; i < stringTokens.size(); i++)
    {
      if ((stringTokens[i]) == "")
        stringTokens.erase(stringTokens.begin() + i);
    }
  }

  void processStringVec()
  {
    vector<string> newVec;
    string stringConstant;
    string extraChars;

    for (auto const &token : stringTokens)
    {
      if (isString(token))
      {
        stringConstant = leaveOnlyQuotations(token);
        extraChars = extraAfterQuotations(token);
        newVec.push_back(stringConstant);
        newVec.push_back(extraChars);
      }
      else
      {
        newVec.push_back(token);
      }
    }
    stringTokens = newVec;
  }

  vector<string> splitVecByChar(vector<string> inputVec, char c)
  {
    vector<string> tempVec;
    vector<string> outputVec;
    for (vector<string>::iterator it = inputVec.begin(); it != inputVec.end(); ++it)
    {
      if (!isString(*it))
      {
        tempVec = splitStringByChar(*it, c);
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
      else
      {
        tempVec = processString(*it);
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
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

  vector<string>
  splitStringAndRemoveByChar(string line, char c)
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

  vector<string> processString(string line)
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
      if (!isString(*it))
      {
        tempVec = splitStringAndRemoveByChar(*it, c);
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
      else
      {
        tempVec = processString(*it);
        outputVec.insert(outputVec.end(), tempVec.begin(), tempVec.end());
      }
    };
    return outputVec;
  }

  //STRING MANIPULATION

  string leaveOnlyQuotations(string str)
  {
    int start = str.find('"');
    int end = str.find_last_of('"');

    return str.substr(start, end + 1);
  }

  string extraAfterQuotations(string str)
  {
    int start = str.find_last_of('"');
    int end = str.length();
    return str.substr(start + 1, end);
  }

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
