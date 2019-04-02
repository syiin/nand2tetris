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
    myTable.startClass();
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
      vector<string> outputVarVec;
      while (tokenString != ";")
      {
        if (tokenString != ",")
          outputVarVec.push_back(tokenString);
        loadNxtToken();
      }

      for (int i = 2; i < outputVarVec.size(); i++)
      {
        myTable.define(outputVarVec[i], outputVarVec[0], outputVarVec[1]); //field int x,y >> (x|y, var, int)
      }

      loadNxtToken(); //;
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

    int i = 0;

    loadNxtToken(); // argument type

    if (myTable.getFunctionType(currentFunctionName) == "method") //handle method
    {
      myTable.define("this", "argument", myTable.getFunctionType(currentFunctionName));
      i = 2;
    }

    while (tokenString != ")") //push back only types and argument names
    {
      if (tokenString != ",")
      {
        outputVarVec.push_back(tokenString);
      }
      loadNxtToken();
    }

    for (; i < outputVarVec.size(); i = i + 2) //handle function | constructor
    {
      myTable.define(outputVarVec[i + 1], "argument", outputVarVec[i]); //(eg. var int x >> arr[i]=int, arr[i+1]=x)
    }

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

    if (myTable.getFunctionType(currentFunctionName) == "constructor")
    {
      myWriter.writePush("constant", to_string(myTable.FieldCount() + lclVar));
      myWriter.writeCall("Memory.alloc", "1");
      myWriter.writePop("pointer", "0"); //allocate memory on heap and give the address of this new memory to pointer 0
    }

    if (myTable.getFunctionType(currentFunctionName) == "method")
    {
      myWriter.writePush("argument", "0");
      myWriter.writePop("pointer", "0"); //pass address of the current object as argument 0 if method
    }

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
      myTable.define(outputVarVec[i], outputVarVec[0], outputVarVec[1]); //var int x,y >> (x|y, var, int)
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

    if (kind == "field")
    {
      myWriter.writePop("this", to_string(idx));
    }
    else
    {
      myWriter.writePop(kind, to_string(idx)); // pop segment idx
    }
    loadNxtToken(); //;
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

    myWriter.writeGoTo(L2); //exit the if loop

    if (tokenString == "else") //handle else case
    {
      loadNxtToken();          //else
      loadNxtToken();          //{
      myWriter.writeLabel(L1); //or exit here
      while (tokenString != "}")
        compileStatements();

      loadNxtToken(); //}
    }
    myWriter.writeLabel(L1); //exit the else loop
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
    myWriter.writeIf(L2); //if-goto L2 - failure case

    loadNxtToken(); //)
    loadNxtToken(); //{

    compileStatements();
    loadNxtToken();          //}
    myWriter.writeGoTo(L1);  //goto L1 - return to the top of while
    myWriter.writeLabel(L2); // label L2 - exit while loop
    labelCounter++;
  }

  void compileDo()
  {
    string outputString = "";
    string className = "";

    int nArgs = 0;

    loadNxtToken();

    //handle defined class instance call (ie. let square = new Square(), square.moveUp())
    if (myTable.subroutineTableContains(tokenString) || myTable.classTableContains(tokenString))
    {
      className = myTable.TypeOf(tokenString);
      string classKind = myTable.KindOf(tokenString);
      string classIdx = to_string(myTable.IndexOf(tokenString));

      myWriter.writePush(classKind, classIdx);
      nArgs++;
      outputString = className;

      loadNxtToken();

      while (tokenString != "(")
      {
        outputString = outputString + tokenString; //  className.methodName
        loadNxtToken();
      }
    }
    else //handle function calls from elsewhere or internal method calls
    {
      while (tokenString != "(")
      {
        if (tokenString == ".")
        {
          className = myTokenizer.lookBehindString(); //get the className from the whole string
        }
        outputString = outputString + tokenString; // class.functionName | functionName

        loadNxtToken();
      }
    }

    if (className == "") //handle if internal method (eg. draw() called within Square class)
    {
      myWriter.writePush("pointer", "0");
      outputString = currentClassName + "." + outputString;
      nArgs++;
    }

    nArgs = nArgs + compileExpressionList();
    myWriter.writeCall(outputString, to_string(nArgs));

    myWriter.writePop("temp", "0");

    loadNxtToken(); //)
    loadNxtToken(); //;
  }

  void compileReturn()
  {
    loadNxtToken(); //return

    if (myTable.getFunctionRtnType(currentFunctionName) == "void") //handle void
    {
      myWriter.writePush("constant", "0");
      myWriter.writeReturn();
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

    while (tokenString != ")")
    {
      loadNxtToken();         // load past '(' and ','
      if (tokenString != ")") // in case "()"
        nArgs++;
      compileExpression();
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
    string lookBehindString = myTokenizer.lookBehindString();
    string outputString = "";

    if (nextTokenString == ".") //this is a method call
    {
      while (tokenString != "(")
      {
        outputString = outputString + tokenString; //class.method
        loadNxtToken();
      }
      int nArgs = compileExpressionList();

      myWriter.writeCall(outputString, to_string(nArgs));
    }
    else if (tokenString == "(")
    {
      loadNxtToken(); //(
      compileExpression();
    }
    else if (tokenString == "[")
    {
      string identifier = lookBehindString;
      string iKind = myTable.KindOf(identifier);
      int iIdx = myTable.IndexOf(identifier);
      myWriter.writePush(iKind, to_string(iIdx)); //push base array base

      loadNxtToken();      //[
      compileExpression(); //returns on ]
      loadNxtToken();      //]

      myWriter.writeArithmetic("add");   //add the expression to the base
      myWriter.writePop("pointer", "1"); //THAT to the address
      myWriter.writePush("that", "0");   //push the array value onto the stack
    }
    else if (tokenType == "identifier" && tokenString != "this") //this is a variable
    {
      string type;

      string kind = myTable.KindOf(tokenString);
      int idx = myTable.IndexOf(tokenString);

      if (kind == "field") //if its a field, access THIS segment
      {
        myWriter.writePush("this", to_string(idx));
      }
      else
      {
        myWriter.writePush(kind, to_string(idx));
      }
    }
    else if (tokenString == "this") //if segment is THIS, push the address that it points to. (eg. return this will save the object address)
    {
      myWriter.writePush("pointer", "0");
    }
    else if (tokenString == "true") //true is represented as -1
    {
      myWriter.writePush("constant", "1");
      myWriter.writeArithmetic("neg");
    }
    else if (tokenString == "false") //false as 0
    {
      myWriter.writePush("constant", "0");
    }
    else if (tokenString == "-" && lookBehindString == "(" && lookBehindString == "[" && lookBehindString == "=") // '-' when not preceded by opening symbols is a neg expression
    {
      myWriter.writePush("constant", nextTokenString);
      myWriter.writeArithmetic("neg");
      loadNxtToken();
    }
    else if (tokenString == "~" && lookBehindType == "symbol") //handle not
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
    else if (tokenType == "stringConstant")
    {
      int stringLen = tokenString.length();
      int charToInt;

      myWriter.writePush("constant", to_string(stringLen));
      myWriter.writeCall("String.new", "1"); //create a new string object of string length

      for (auto const &character : tokenString)
      {
        int charToInt = character;
        myWriter.writePush("constant", to_string(charToInt));
        myWriter.writeCall("String.appendChar", "2"); //call each ascii rep of char and append to string object
      }
    }
    else if (tokenType == "symbol" && checkIfOps(tokenString))
    {
      string opCmd = arithOpsTable[tokenString];
      loadNxtToken();
      compileTerm();
      myWriter.writeArithmetic(opCmd); //recursive postfix operations handling
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
