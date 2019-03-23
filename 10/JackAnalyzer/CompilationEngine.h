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
    outputFile << createTagLoadNext(tokenType, tokenString)
               << createTagLoadNext(tokenType, tokenString)
               << createTagLoadNext(tokenType, tokenString);
    if (tokenString != "function" || tokenString != "method" || tokenString != "constructor")
    {
      compileClassVarDec();
    }
    else
    {
      compileSubroutineDec();
    }
    outputFile << createTagLoadNext(tokenType, tokenString); //}
    outputFile << "</class>" << endl;
  }

  void compileClassVarDec()
  {
    outputFile << "<classVarDec>" << endl;
    while (tokenString != "function" && tokenString != "method" && tokenString != "constructor")
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << "</classVarDec>" << endl;

    while (tokenString != "}")
    {
      compileSubroutineDec();
    }
  }

  void compileSubroutineDec()
  {
    outputFile << "<subroutineDec>" << endl;
    while (tokenString != "(" && tokenString != "}")
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << createTagLoadNext(tokenType, tokenString); // (
    compileParameterList();
    outputFile << "</subroutineDec>" << endl;
  }

  void compileParameterList()
  {
    outputFile << "<parameterList>" << endl;
    while (tokenString != ")" && tokenString != "}")
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << "</parameterList>" << endl;
    outputFile << createTagLoadNext(tokenType, tokenString); //{

    compileSubroutineBody();
  }

  void compileSubroutineBody()
  {
    outputFile << "<subroutineBody>" << endl;
    outputFile << createTagLoadNext(tokenType, tokenString);

    while (tokenString == "var")
    {
      compileVarDec();
    }

    while (tokenString != "}")
    {
      compileStatements();
    }
    outputFile << createTagLoadNext(tokenType, tokenString);
    outputFile << "</subroutineBody>" << endl;
  }

  void compileVarDec()
  {
    outputFile << "<varDec>" << endl;
    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << createTagLoadNext(tokenType, tokenString);
    outputFile << "</varDec>" << endl;
  }

  void compileStatements()
  {
    outputFile << "<statements>" << endl;
    while (tokenString != "}")
    {
      if (tokenString == "let")
        compileLet();
      else if (tokenString == "if")
      {
        compileIf();
      }
      else if (tokenString == "while")
      {
        compileWhile();
      }
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

    while (tokenString != "=")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //let a
      if (tokenString == "[")
      {
        outputFile << createTagLoadNext(tokenType, tokenString); //[
        compileExpression();
        outputFile << createTagLoadNext(tokenType, tokenString); //]
      }
    };

    outputFile << createTagLoadNext(tokenType, tokenString); //=
    compileExpression();
    outputFile << createTagLoadNext(tokenType, tokenString); //;

    outputFile << "</letStatement>" << endl;
  }

  void compileIf()
  {
    outputFile << "<ifStatement>" << endl;

    outputFile << createTagLoadNext(tokenType, tokenString); //if
    outputFile << createTagLoadNext(tokenType, tokenString); //(

    compileExpression();

    outputFile << createTagLoadNext(tokenType, tokenString); // )
    outputFile << createTagLoadNext(tokenType, tokenString); // {

    compileStatements();
    outputFile << createTagLoadNext(tokenType, tokenString); //}

    if (tokenString == "else")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //else
      outputFile << createTagLoadNext(tokenType, tokenString); //{
      while (tokenString != "}")
        compileStatements();
      outputFile << createTagLoadNext(tokenType, tokenString); //}
    }
    outputFile << "</ifStatement>" << endl;
  }

  void compileWhile()
  {
    outputFile << "<whileStatement>" << endl;

    outputFile << createTagLoadNext(tokenType, tokenString)  //while
               << createTagLoadNext(tokenType, tokenString); //(

    compileExpression();

    outputFile << createTagLoadNext(tokenType, tokenString); //)
    outputFile << createTagLoadNext(tokenType, tokenString); //{
    compileStatements();
    outputFile << createTagLoadNext(tokenType, tokenString); //}
    outputFile << "</whileStatement>" << endl;
  }

  void compileDo()
  {
    outputFile << "<doStatement>" << endl;

    outputFile << createTagLoadNext(tokenType, tokenString); //do
    outputFile << createTagLoadNext(tokenType, tokenString); //class
    outputFile << createTagLoadNext(tokenType, tokenString); //.
    outputFile << createTagLoadNext(tokenType, tokenString); //method
    outputFile << createTagLoadNext(tokenType, tokenString); //(
    compileExpressionList();
    outputFile << createTagLoadNext(tokenType, tokenString); //)
    outputFile << createTagLoadNext(tokenType, tokenString); //;

    outputFile << "</doStatement>" << endl;
  }

  void compileReturn()
  {
    outputFile << "<returnStatement>" << endl;
    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << createTagLoadNext(tokenType, tokenString);
    outputFile << "</returnStatement>" << endl;
  }

  void compileExpressionList()
  {
    outputFile << "<expressionList>" << endl;
    while (tokenString != ")" && tokenString != ";")
    {
      compileExpression();
    }
    outputFile << "</expressionList>" << endl;
  }

  void compileExpression()
  {
    outputFile << "<expression>" << endl;
    while (tokenString != "," && tokenString != ";" && tokenString != ")" && tokenString != "]")
    {
      if (myTokenizer.lookBehindString() == "(" && tokenType == "symbol")
      {
        compileTerm();
      }
      else if (tokenType == "symbol" && tokenString != "(")
      {
        outputFile << createTagLoadNext(tokenType, tokenString);
      }
      else
      {
        compileTerm();
      }
    }
    outputFile << "</expression>" << endl;
  }

  void compileTerm()
  {
    outputFile << "<term>" << endl;

    string nextTokenString = myTokenizer.lookAheadString();
    if (nextTokenString == ".")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); // class
      outputFile << createTagLoadNext(tokenType, tokenString); // .
      outputFile << createTagLoadNext(tokenType, tokenString); // method
      outputFile << createTagLoadNext(tokenType, tokenString); // (
      compileExpressionList();
      outputFile << createTagLoadNext(tokenType, tokenString); // )
    }
    else if (nextTokenString == "[")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //identifier
      outputFile << createTagLoadNext(tokenType, tokenString); //[
      compileExpression();
      outputFile << createTagLoadNext(tokenType, tokenString); //]
    }
    else if (tokenString == "(")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //(
      compileExpression();
      outputFile << createTagLoadNext(tokenType, tokenString); //)
    }
    else if (myTokenizer.lookBehindString() == "(" && tokenType == "symbol")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //symbol
      compileTerm();                                           //<term>identifier</term>
    }
    else
    {
      outputFile << createTagLoadNext(tokenType, tokenString);
    }
    outputFile << "</term>" << endl;
  }

  void setOutputFileName(string outputFileString)
  {
    outputFileName = outputFileString;
  }

  void loadNxtToken()
  {
    tokenType = myTokenizer.tokenType();
    if (myTokenizer.tokenType() == "keyword")
    {
      tokenString = myTokenizer.keyWord();
    }
    else if (myTokenizer.tokenType() == "symbol")
    {
      tokenString = myTokenizer.symbol();
    }
    else if (myTokenizer.tokenType() == "stringConstant")
    {
      tokenString = myTokenizer.stringVal();
    }
    else if (myTokenizer.tokenType() == "integerConstant")
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