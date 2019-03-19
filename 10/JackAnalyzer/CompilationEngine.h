#ifndef COMPILATIONENG_H
#define COMPILATIONENG_H

#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <experimental/filesystem>

#include "Tokenizer.h"

using namespace std;

class CompilationEngine
{
private:
  ofstream outputFile;
  Tokenizer myTokenizer;

public:
  void loadEngine(string inputFileName)
  {
    myTokenizer.loadTokens(inputFileName);
    // myTokenizer.printTokens();
    translateCode();
  }

  void translateCode()
  {
    cout << "Translating" << endl;
    while (myTokenizer.hasMoreTokens())
    {
      if (myTokenizer.tokenType() == "KEYWORD")
      {
        cout << "KEYWORD" << endl;
      }
      else if (myTokenizer.tokenType() == "SYMBOL")
      {
        cout << "SYMBOL" << endl;
      }
      else if (myTokenizer.tokenType() == "STRING_CONST")
      {
        cout << "STRING_CONST" << endl;
      }
      else if (myTokenizer.tokenType() == "INT_CONST")
      {
        cout << "INT_CONST" << endl;
      }
      else if (myTokenizer.tokenType() == "IDENTIFIER")
      {
        cout << "IDENTIFIER" << endl;
      }
      else
      {
        cout << "UNKNOWN WORD" << endl;
      }
      myTokenizer.advance();
    }
  }

  void compileClass()
  {
  }

  void compileClassVarDec()
  {
  }

  void compileSubroutineDec()
  {
  }
};

#endif