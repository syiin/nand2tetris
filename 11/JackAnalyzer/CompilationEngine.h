#ifndef COMPILATIONENG_H
#define COMPILATIONENG_H

#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <experimental/filesystem>

#include "Tokenizer.h"
#include "SymbolTable.h"
#include "VMWriter.h"

using namespace std;

class CompilationEngine
{
private:
  ofstream outputFile;
  string outputFileName;
  string currentClassName;

  Tokenizer myTokenizer;
  VMWriter myWriter;
  SymbolTable myTable;
  map<string, string> arithOpsTable;

  string tokenString;
  string tokenType;

public:
  void loadEngine(string inputFileName)
  {
    myTokenizer.loadTokens(inputFileName);
    myWriter.startWriter(outputFileName);
    initArithOpsTable();
    // myTokenizer.printTokens();
    compileClass();
  }

  void compileClass()
  {
    loadNxtToken();                   //empty space
    loadNxtToken();                   //class
    setCurrentClassName(tokenString); //class identifier
    loadNxtToken();                   //{
    loadNxtToken();                   //function OR var

    if (!checkIfFunction())
    {
      compileClassVarDec();
    }
    else
    {
      compileSubroutineDec();
    }

    loadNxtToken(); //}
  }

  void compileClassVarDec()
  {
    cout << tokenString << endl;
    cout << !checkIfFunction() << endl;

    while (!checkIfFunction())
    {
      // vector<string> outputVarVec;
      // while (tokenString != ";")
      // {
      //   outputVarVec.push_back(tokenString);
      //   loadNxtToken();
      // }
      // myTable.define(outputVarVec[2], outputVarVec[1], outputVarVec[0]);
    }

    myTable.printTable();

    while (tokenString != "}")
    {
      compileSubroutineDec();
    }
  }

  void compileSubroutineDec()
  {
    string outputString;

    loadNxtToken(); //return type
    loadNxtToken(); //identifier
    outputString = currentClassName + "." + tokenString;

    loadNxtToken(); //(

    compileParameterList(outputString);

    loadNxtToken(); // (
  }

  void compileParameterList(string functionName)
  {
    myTable.startSubroutine();
    string name;
    string type;
    int paramIdx = 0;
    int paramCount = 0;

    while (tokenString != ")")
    {
      if (paramIdx == 0)
      {
        type = tokenString;
        paramIdx++;
      }
      else if (paramIdx == 1)
      {
        name = tokenString;
        paramIdx++;
      }
      else if (paramIdx == 2)
      {
        myTable.define(name, "argument", type);
        paramIdx = 0;
      }
      loadNxtToken();
      paramCount++;
    }
    myWriter.writeFunction(functionName, paramCount / 2);

    loadNxtToken(); // )

    compileSubroutineBody();
  }

  void compileSubroutineBody()
  {

    loadNxtToken(); //{

    while (tokenString == "var")
    {
      compileVarDec();
    }

    while (tokenString != "}")
    {
      compileStatements();
    }

    loadNxtToken(); //}
  }

  void compileVarDec()
  {
    vector<string> outputVarVec;
    while (tokenString != ";")
    {
      outputVarVec.push_back(tokenString);
      loadNxtToken();
    }
    myTable.define(outputVarVec[2], outputVarVec[0], outputVarVec[1]);
    loadNxtToken(); //;
  }

  void compileStatements()
  {
    while (tokenString != "}")
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
  }

  void compileLet()
  {
    int idx;
    string type;
    string kind;
    while (tokenString != "=")
    {
      if (tokenString == "[")
      {
        loadNxtToken(); // [
        compileExpression();
        loadNxtToken(); // ]
      }
      idx = myTable.IndexOf(tokenString);
      type = myTable.TypeOf(tokenString);
      kind = myTable.KindOf(tokenString);
      loadNxtToken();
    }
    loadNxtToken(); //=
    compileExpression();
    myTable.printTable();
    myWriter.writePop(kind, idx); // pop segment idx
    loadNxtToken();               //;
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

    if (tokenString == "else") //handle else case
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
    string outputString;
    string className;
    string methodName;
    int nArgs;

    loadNxtToken();          //do
    className = tokenString; //Square
    loadNxtToken();
    loadNxtToken();           // .
    methodName = tokenString; //main
    loadNxtToken();           //(

    nArgs = compileExpressionList();

    myWriter.writeCall(className + "." + methodName, nArgs);
    loadNxtToken(); //)
    loadNxtToken(); //;
  }

  void compileReturn()
  {
    outputFile << "<returnStatement>" << endl;
    while (tokenString != ";")
    {
      outputFile << createTagLoadNext(tokenType, tokenString); //return xyz
    }
    outputFile << createTagLoadNext(tokenType, tokenString); //;
    outputFile << "</returnStatement>" << endl;
  }

  int compileExpressionList()
  {
    int nArgs = 0;
    while (tokenString != ")" && tokenString != ";")
    {
      loadNxtToken(); // ,
      compileExpression();
      nArgs++;
    }
    return nArgs;
  }

  void compileExpression()
  {
    string outputString;
    vector<string> pushVec;
    vector<string> afterVec;

    while (checkEndOfExpression())
    {
      outputString = compileTerm();
      if (tokenType == "integerConstant")
      {
        pushVec.push_back(outputString);
      }
      else if (tokenType == "symbol")
      {
        afterVec.push_back(outputString);
      }
      loadNxtToken();
    }

    for (auto const &term : pushVec)
    {
      cout << "term " << term << endl;
      myWriter.writePush("constant", term);
    }

    for (auto const &op : afterVec)
    {
      cout << "OP" << op << endl;
      // THE FUNCTION CALLS (ie. MEMORY.PEEK) ARE SOMEHOW BEING RECOGNISED AS SYMBOLS?
      myWriter.writeArithmetic(op);
    }
  }

  string compileTerm()
  {
    string nextTokenString = myTokenizer.lookAheadString();
    string outputString;
    if (nextTokenString == ".")
    {
      while (tokenString != "(")
      {
        outputString = outputString + tokenString; //class.method
        loadNxtToken();
      }
      loadNxtToken(); // (
      int nArgs = compileExpressionList();
      loadNxtToken(); // )
      // myWriter.writeCall(outputString, nArgs);
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
    else if (tokenType == "symbol" && tokenString != "(")
    {
      outputString = arithOpsTable[tokenString];
      return outputString;
    }
    else if (tokenType == "integerConstant")
    {
      outputString = tokenString;
      return outputString;
    }
  }

  void setOutputFileName(string outputFileString)
  {
    outputFileName = outputFileString;
  }

  void setCurrentClassName(string currentClassNameString)
  {
    currentClassName = currentClassNameString;
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
      string tempString = myTokenizer.stringVal();
      int start = myTokenizer.stringVal().find('"') + 1;
      int end = myTokenizer.stringVal().find_last_of('"') - 1;
      tempString = tempString.substr(start, end);

      tokenString = tempString;
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

  string getStringLoadNext()
  {
    string outputString = tokenString;
    loadNxtToken();
    return outputString;
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

  //CHECKERS

  bool checkIfFunction()
  {
    if (tokenString == "function")
      return true;
    else if (tokenString == "method")
      return true;
    else if (tokenString == "constructor")
      return true;
    else
      return false;
  }
  bool checkEndOfExpression()
  {
    return (tokenString != "," && tokenString != ";" && tokenString != ")" && tokenString != "]");
  }

  //INITIALIZERS
  void initArithOpsTable()
  {
    arithOpsTable["+"] = "add";
    arithOpsTable["*"] = "call Math.multiply 2";
    arithOpsTable["-"] = "neg";
  };
};

#endif