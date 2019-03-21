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

  string tokenString;
  string tokenType;

public:
  void loadEngine(string inputFileName)
  {
    myTokenizer.loadTokens(inputFileName);
    outputFile.open(outputFileName + ".xml");
    compileClass();
    // myTokenizer.printTokens();
  }

  void compileClass()
  {
    outputFile << "<class>" << endl;
    loadNxtToken();
    outputFile << createTagLoadNext("keyword", tokenString)
               << createTagLoadNext("identifier", tokenString)
               << createTagLoadNext("symbol", tokenString);
    compileClassVarDec();
    outputFile << "</class>" << endl;
  }

  void compileClassVarDec()
  {
    outputFile << "<classVarDec>" << endl;
    while (tokenString != "function" || tokenString != "method" || tokenString != "constructor")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);

      while (tokenString != ";")
      {
        outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
      }
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << "</classVarDec>" << endl;
  }

  void compileSubroutineDec()
  {
    outputFile << "<subroutineDec>" << endl;

    while (tokenString != "}")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);

    compileParameterList();
    outputFile << "</subroutineDec>" << endl;
  }

  void compileParameterList()
  {
    outputFile << "<parameterList>" << endl;

    while (tokenString != ")")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);

    compileSubroutineBody();
    outputFile << "</parameterList>" << endl;
  }

  void compileSubroutineBody()
  {
    outputFile << "<subroutineBody>" << endl
               << createTagLoadNext("symbol", tokenString)
               << "</subroutineBody>" << endl;
    while (tokenString != "}")
    {
      if (tokenString == "var")
        compileVarDec();
      else if (tokenString == "let")
        compileLet();
      else if (tokenString == "if")
        compileIf();
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
  }

  void compileVarDec()
  {
    outputFile << "<varDec>" << endl;
    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    outputFile << "</varDec>" << endl;
  }

  void compileStatements()
  {
    outputFile << "<statements>" << endl;
    while (tokenString != ";")
    {
      if (tokenString == "let")
        compileLet();
      else if (tokenString == "if")
        compileIf();
      else if (tokenString == "while")
        compileWhile();
      else if (tokenString == "do")
        compileDo();
      else if (tokenString == "return")
        compileReturn();
    }
    outputFile << "</statements>" << endl;
  }

  void compileLet()
  {
    outputFile << "<letStatement>" << endl;

    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);

    compileExpression();

    outputFile << "</letStatement>" << endl;
  }

  void compileIf()
  {
    outputFile << "<ifStatement>" << endl;
    while (tokenString != "}")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    outputFile << "</ifStatement>" << endl;
  }

  void compileWhile()
  {
    outputFile << "<whileStatement>" << endl;
    while (tokenString != "}")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    outputFile << "</whileStatement>" << endl;
  }

  void compileDo()
  {
    outputFile << "<doStatement>" << endl;
    while (tokenString != "}")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    outputFile << "</whileStatement>" << endl;
  }

  void compileReturn()
  {
    outputFile << "<returnStatement>" << endl;
    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    }
    outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
    outputFile << "</returnStatement>" << endl;
  }

  void compileExpression()
  {
    while (tokenString != ";")
    {
      compileTerm();
    }
  }

  void compileTerm()
  {
    outputFile << "<term>" << endl;
    string currentToken = tokenString;
    loadNxtToken();
    string nextToken = tokenString;

    while (tokenString != ";")
    {
      if (nextToken == "[")
      {
        outputFile << createFullTag("symbol", "[");
        compileExpression();
        outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
      }
      else if (nextToken == ".")
      {
        outputFile << createFullTag("symbol", ".")
                   << createTagLoadNext(myTokenizer.tokenType(), tokenString);
      }
      else if (nextToken == "(")
      {
        outputFile << createFullTag("symbol", "(");
        compileExpression();
        outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
      }
      else
      {
        outputFile << createTagLoadNext(myTokenizer.tokenType(), tokenString);
      }
    }
    outputFile << "</term>" << endl;
  }

  void setOutputFileName(string outputFileString)
  {
    outputFileName = outputFileString;
  }

  void loadNxtToken()
  {
    if (myTokenizer.tokenType() == "keyword")
    {
      tokenString = myTokenizer.keyWord();
    }
    else if (myTokenizer.tokenType() == "symbol")
    {
      tokenString = myTokenizer.symbol();
    }
    else if (myTokenizer.tokenType() == "string_const")
    {
      tokenString = myTokenizer.stringVal();
    }
    else if (myTokenizer.tokenType() == "int_const")
    {
      tokenString = myTokenizer.intVal();
    }
    else if (myTokenizer.tokenType() == "identifier")
    {
      tokenString = myTokenizer.identifier();
    }
    else
    {
      cout << "UNKNOWN WORD" << endl;
    }
    myTokenizer.advance();
  }

  string createTagLoadNext(string tagName, string tagValue)
  {
    string outputString = createFullTag(tagName, tagValue);
    loadNxtToken();
    return outputString;
  }

  string createFullTag(string tagName, string tagValue)
  {
    string outputString;
    outputString = "<" + tagName;
    outputString += "> " + tagValue + " </" + tagName + ">\n";
    return outputString;
  }
};

#endif