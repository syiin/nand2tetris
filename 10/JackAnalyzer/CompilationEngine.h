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
  string outputFileName;
  Tokenizer myTokenizer;

public:
  void loadEngine(string inputFileName)
  {
    myTokenizer.loadTokens(inputFileName);
    outputFile.open(outputFileName + ".xml");
    compileClass();
    myTokenizer.printTokens();
  }

  void compileClass()
  {
    outputFile << "<tokens>" << endl;
    while (myTokenizer.hasMoreTokens())
    {
      if (myTokenizer.tokenType() == "KEYWORD")
      {
        outputFile << createTag("keyword", myTokenizer.keyWord());
      }
      else if (myTokenizer.tokenType() == "SYMBOL")
      {
        outputFile << createTag("symbol", myTokenizer.symbol());
      }
      else if (myTokenizer.tokenType() == "STRING_CONST")
      {
        outputFile << createTag("stringConstant", myTokenizer.stringVal());
      }
      else if (myTokenizer.tokenType() == "INT_CONST")
      {
        outputFile << createTag("integerConstant", myTokenizer.intVal());
      }
      else if (myTokenizer.tokenType() == "IDENTIFIER")
      {
        outputFile << createTag("identifier", myTokenizer.identifier());
      }
      else
      {
        cout << "UNKNOWN WORD" << endl;
      }
      myTokenizer.advance();
    }
    outputFile << "</tokens>" << endl;
  }

  void compileClassVarDec()
  {
  }

  void compileSubroutineDec()
  {
  }

  void setOutputFileName(string outputFileString)
  {
    outputFileName = outputFileString;
  }

  string createTag(string tagName, string tagValue)
  {
    string outputString;
    outputString = "<" + tagName;
    outputString += "> " + tagValue + " </" + tagName + ">\n";
    return outputString;
  }
};

#endif