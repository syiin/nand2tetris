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
  string currentFunctionName;

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
    myWriter.close();
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
    while (tokenString == "field")
    {
      compileVarDec();
    }

    while (tokenString != "}")
    {
      compileSubroutineDec();
    }
  }

  void compileSubroutineDec()
  {
    string outputString;
    string rtnType;
    string functionType;

    functionType = tokenString; // function | method | constructor
    loadNxtToken();
    rtnType = tokenString; //return type int | char | identifier

    loadNxtToken(); //identifier
    outputString = currentClassName + "." + tokenString;

    myTable.addFunction(outputString, functionType, rtnType);
    currentFunctionName = outputString;

    loadNxtToken(); //(
    compileParameterList();
  }

  void compileParameterList()
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

    if (myTable.getFunctionType(currentFunctionName) == "method") //handle method
    {
      myTable.define("this", "argument", "0");
    }

    for (int i = 0; i < outputVarVec.size(); i = i + 2) //handle function | constructor
    {
      myTable.define(outputVarVec[i + 1], "argument", outputVarVec[i]);
    }

    myTable.printTable();

    loadNxtToken(); // )
    compileSubroutineBody();
    loadNxtToken(); // }
  }

  void compileSubroutineBody()
  {
    loadNxtToken(); //{
    int lclVar = 0;
    while (tokenString == "var")
    {
      lclVar = lclVar + compileVarDec();
    }
    myWriter.writeFunction(currentFunctionName, lclVar);

    while (tokenString != "}")
    {
      compileStatements();
    }
  }

  int compileVarDec()
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
    }

    loadNxtToken(); //;

    return outputVarVec.size() - 2;
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
    myWriter.writePop(kind, to_string(idx)); // pop segment idx
    loadNxtToken();                          //;
  }

  void compileIf()
  {
    string L1 = "L" + to_string(labelCounter);
    labelCounter++;
    string L2 = "L" + to_string(labelCounter);
    labelCounter++;

    loadNxtToken(); //if
    loadNxtToken(); //(
    compileExpression();
    loadNxtToken(); // )

    myWriter.writeArithmetic("not");

    myWriter.writeIf(L1);
    loadNxtToken(); // {
    compileStatements();
    loadNxtToken(); //}

    myWriter.writeGoTo(L2);

    if (tokenString == "else") //handle else case
    {
      loadNxtToken(); //else
      loadNxtToken(); //{
      myWriter.writeLabel(L1);
      while (tokenString != "}")
        compileStatements();

      loadNxtToken(); //}
    }
    myWriter.writeLabel(L2);
  }

  void compileWhile()
  {
    string L1 = "L" + to_string(labelCounter);
    labelCounter++;
    string L2 = "L" + to_string(labelCounter);
    labelCounter++;

    myWriter.writeLabel(L1); // label L1
    labelCounter++;

    loadNxtToken(); //while
    loadNxtToken(); //(

    compileExpression();
    myWriter.writeArithmetic("not");
    myWriter.writeIf(L2); //if-goto L2

    loadNxtToken(); //)
    loadNxtToken(); //{

    compileStatements();
    loadNxtToken();          //}
    myWriter.writeGoTo(L1);  //goto L1
    myWriter.writeLabel(L2); // label L2
    labelCounter++;
  }

  void compileDo()
  {
    string outputString;
    string className = "";
    string methodName;
    int nArgs;

    loadNxtToken();
    while (tokenString != "(")
    {
      className = className + tokenString; // class.functionName | functionName
      loadNxtToken();
    }

    nArgs = compileExpressionList();
    myWriter.writeCall(className, nArgs);
    loadNxtToken(); //)
    loadNxtToken(); //;
  }

  void compileReturn()
  {
    loadNxtToken(); //return
    string currentFunctionRtnType = myTable.getFunctionRtnType(currentFunctionName);

    if (currentFunctionRtnType == "void") //handle void vs other return types
    {
      myWriter.writePush("constant", "0");
      myWriter.writeReturn();
      myWriter.writePop("temp", "0");
    }
    else
    {
      compileExpression(); //push return value onto stack
      myWriter.writeReturn();
    }

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
    while (checkEndOfExpression())
    {
      compileTerm(); //compile any terms inside
      loadNxtToken();
    }
  }

  void compileTerm()
  {
    string nextTokenString = myTokenizer.lookAheadString();
    string nextTokenType = myTokenizer.lookAheadType();
    string lookBehindType = myTokenizer.lookBehindType();
    string outputString = "";

    // cout << tokenString << endl;
    // cout << nextTokenString << endl;

    if (nextTokenString == ".") //this is a method call
    {
      while (tokenString != "(")
      {
        outputString = outputString + tokenString; //class.method
        loadNxtToken();
      }
      int nArgs = compileExpressionList();

      myWriter.writeCall(outputString, nArgs);
    }
    else if (tokenString == "(")
    {
      loadNxtToken(); //(
      compileExpression();
    }
    else if (tokenType == "identifier") //this is a variable
    {
      int idx;
      string type;
      string kind;

      idx = myTable.IndexOf(tokenString);
      kind = myTable.KindOf(tokenString);

      myWriter.writePush(kind, to_string(idx));
    }
    else if (tokenString == "true")
    {
      myWriter.writePush("constant", "1");
      myWriter.writeArithmetic("neg");
    }
    else if (tokenString == "false")
    {
      myWriter.writePush("constant", "0");
    }
    else if (tokenString == "-" && lookBehindType == "symbol")
    {
      myWriter.writePush("constant", nextTokenString);
      myWriter.writeArithmetic("neg");
      loadNxtToken();
    }
    else if (tokenString == "~" && lookBehindType == "symbol")
    {
      loadNxtToken(); // (
      compileExpression();
      loadPrevToken();
      myWriter.writeArithmetic("not");
    }
    else if (tokenType == "integerConstant") //this is just an integer
    {
      myWriter.writePush("constant", tokenString);
    }
    else if (tokenType == "symbol" && checkIfOps(tokenString))
    {
      string opCmd = arithOpsTable[tokenString];
      loadNxtToken();
      compileTerm();
      myWriter.writeArithmetic(opCmd);
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

  void loadPrevToken()
  {
    myTokenizer.pointerBack();
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

  bool checkIfOps(string key)
  {
    for (auto const &symbol : arithOpsTable)
    {
      if (symbol.first == key)
      {
        return true;
      }
    }
    return false;
  }

  //INITIALIZERS
  void initArithOpsTable()
  {
    arithOpsTable["+"] = "add";
    arithOpsTable["-"] = "sub";
    arithOpsTable["*"] = "call Math.multiply 2";
    arithOpsTable["/"] = "call Math.divide 2";
    arithOpsTable[">"] = "gt";
    arithOpsTable["<"] = "lt";
    arithOpsTable["="] = "eq";
    arithOpsTable["&"] = "and";
    arithOpsTable["|"] = "or";
  };
};

#endif

/*

 
*/
