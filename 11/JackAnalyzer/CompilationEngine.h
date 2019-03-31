#ifndef COMPILATIONENG_H
#define COMPILATIONENG_H

#include <map>
#include <tuple>
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
  int labelCounter;

public:
  void loadEngine(string inputFileName)
  {
    labelCounter = 0;
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
      while (tokenString != "}")
      {
        compileSubroutineDec();
      }
    }

    loadNxtToken(); //}
  }

  void compileClassVarDec()
  {
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
  }

  void compileParameterList(string functionName)
  {
    myTable.startSubroutine();
    string name;
    string type;
    vector<string> outputVarVec;

    int paramCount = 0;

    loadNxtToken(); // argument type

    while (tokenString != ")")
    {
      if (tokenString != ",")
      {
        outputVarVec.push_back(tokenString);
        paramCount++;
      }
      loadNxtToken();
    }

    for (int i = 0; i < outputVarVec.size(); i = i + 2)
    {
      myTable.define(outputVarVec[i + 1], "argument", outputVarVec[i]);
    }

    myWriter.writeFunction(functionName, paramCount / 2);

    loadNxtToken(); // )
    compileSubroutineBody();
    loadNxtToken(); // }
  }

  void compileSubroutineBody()
  {
    loadNxtToken(); //{
    while (tokenString == "var")
    {
      compileVarDec();
    }

    myTable.printTable();

    while (tokenString != "}")
    {
      compileStatements();
    }
  }

  void compileVarDec()
  {
    vector<string> outputVarVec;
    while (tokenString != ";")
    {
      if (tokenString != ",")
        outputVarVec.push_back(tokenString);
      loadNxtToken();
    }

    for (int i = 2; i < outputVarVec.size(); i++)
    {
      myTable.define(outputVarVec[i], outputVarVec[0], outputVarVec[1]);
      // cout << outputVarVec[0] << '\t';
      // cout << outputVarVec[1] << '\t';
      // cout << outputVarVec[i] << endl;
    }

    loadNxtToken(); //;
    // myWriter.writeComment(outputVarVec[2] + outputVarVec[0] + outputVarVec[1]);
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
      kind = myTable.KindOf(tokenString);
      loadNxtToken();
    }
    loadNxtToken(); //=
    compileExpression();
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
    myWriter.writeLabel("L" + to_string(labelCounter)); // label L0
    labelCounter++;

    loadNxtToken(); //while
    loadNxtToken(); //(

    compileExpression();
    myWriter.writeIf("L" + to_string(labelCounter)); //if-goto L1

    loadNxtToken(); //)
    loadNxtToken(); //{

    compileStatements();
    loadNxtToken(); //}

    myWriter.writeLabel("L" + to_string(labelCounter)); // label L1
    labelCounter++;
  }

  void compileDo()
  {
    string outputString;
    string className;
    string methodName;
    int nArgs;

    loadNxtToken();          //do
    className = tokenString; //Square
    loadNxtToken();          // .
    loadNxtToken();          //main

    methodName = tokenString;

    loadNxtToken(); //(

    nArgs = compileExpressionList();

    myWriter.writeCall(className + "." + methodName, nArgs);
    loadNxtToken(); //)
    loadNxtToken(); //;
  }

  void compileReturn()
  {
    loadNxtToken(); //return

    compileExpression(); //push return value onto stack
    myWriter.writeReturn();
    loadNxtToken(); // ;
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
    tuple<string, string, string> outputTuple;
    string expressionFrom;
    string outputString;
    string nArgs;

    vector<tuple<string, string>> pushVec;
    vector<string> afterVec;

    while (checkEndOfExpression())
    {
      outputTuple = compileTerm();

      expressionFrom = get<0>(outputTuple);
      outputString = get<1>(outputTuple);
      nArgs = get<2>(outputTuple);

      if (expressionFrom == "constant" || expressionFrom == "boolean")
      {
        tuple<string, string> pushTuple;
        pushTuple = make_tuple(expressionFrom, outputString);
        pushVec.push_back(pushTuple);
      }
      else if (expressionFrom == "arithOperator")
      {
        afterVec.push_back(outputString);
      }
      else if (expressionFrom == "methodCall")
      {
        myWriter.writeCall(outputString, stoi(nArgs));
      }
      else if (expressionFrom == "identifier")
      {
        tuple<string, string> pushTuple;
        pushTuple = make_tuple(outputString, nArgs);
        pushVec.push_back(pushTuple);
      }
      loadNxtToken();
    }

    for (auto const &term : pushVec)
    {
      myWriter.writePush(get<0>(term), get<1>(term));
    }

    for (auto const &op : afterVec)
    {
      myWriter.writeArithmetic(op);
    }
  }

  tuple<string, string, string> compileTerm()
  {
    string nextTokenString = myTokenizer.lookAheadString();
    string outputString;
    tuple<string, string, string> outputTuple;
    if (nextTokenString == ".")
    {
      while (tokenString != "(")
      {
        outputString = outputString + tokenString; //class.method
        loadNxtToken();
      }
      int nArgs = compileExpressionList();

      outputTuple = make_tuple("methodCall", outputString, to_string(nArgs));
      return outputTuple;
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
      outputTuple = make_tuple("arithOperator", outputString, "");
      return outputTuple;
    }
    else if (tokenType == "integerConstant")
    {
      outputTuple = make_tuple("constant", tokenString, "");
      return outputTuple;
    }
    else if (tokenType == "identifier")
    {
      int idx;
      string type;
      string kind;

      idx = myTable.IndexOf(tokenString);
      kind = myTable.KindOf(tokenString);

      outputTuple = make_tuple("identifier", kind, to_string(idx));
      return outputTuple;
    }
    else if (tokenString == "true")
    {
      outputTuple = make_tuple("boolean", "-1", "");
      return outputTuple;
    }
    else if (tokenString == "false")
    {
      outputTuple = make_tuple("boolean", "0", "");
      return outputTuple;
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