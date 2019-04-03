/*

void compileExpression()
  {
    while (checkEndOfExpression())
    {
      compileTerm();

      loadNxtToken();
      if (tokenType == "symbol" && checkTableContainsKey(arithOpsTable, tokenString))
      {
        string opCmd = "";
        opCmd = arithOpsTable[tokenString];

        compileTerm();
        myWriter.writeArithmetic(opCmd);
      }
    }
  }

  void compileTerm()
  {
    loadNxtToken();

    if (tokenType == "identifier")
    {
      string tempString = tokenString;
      loadNxtToken();

      if (tokenType == "symbol" && tokenString == "[")
      {
        myWriter.writePush(myTable.KindOf(tempString), to_string(myTable.IndexOf(tempString)));
        compileExpression();
        loadNxtToken(); //]

        myWriter.writeArithmetic("add");
        myWriter.writePop("pointer", 1);
        myWriter.writePush("that", "0");
      }
      else if (tokenType == "symbol" && tokenString == "(")
      {
        loadPrevToken();
        loadPrevToken();
        compileSubroutineCall();
      }
      else
      {
        loadPrevToken();
        myWriter.writePush(myTable.KindOf(tempString), to_string(myTable.IndexOf(tempString)));
      }
    }
    else
    {
      if (tokenType == "integerConstant")
      {
        myWriter.writePush("constant", tokenString);
      }
      else if (tokenType == "stringConstant")
      {
        string str = tokenString;
        myWriter.writePush("constant", to_string(str.length()));
        myWriter.writeCall("String.new", 1);

        for (int i = 0; i < str.length(); i++)
        {
          myWriter.writePush("constant", to_string((int)str[i]));
          myWriter.writeCall("String.appendChar", 2);
        }
      }
      else if (tokenType == "keyword" && tokenString == "true")
      {
        myWriter.writePush("constant", "0");
        myWriter.writeArithmetic("not");
      }
      else if (tokenType == "keyword" && tokenString == "false")
      {
        myWriter.writePush("constant", "0");
      }
      else if (tokenType == "keyword" && tokenString == "this")
      {
        myWriter.writePush("pointer", "0");
      }
      else if (tokenType == "symbol" && tokenString == "(")
      {
        compileExpression();
        loadNxtToken(); // )
      }
      else if (tokenType == "symbol" && (tokenString == "-" || tokenString == "~"))
      {
        string str = tokenString;

        compileTerm();

        if (str == "-")
          myWriter.writeArithmetic("neg");
        else
          myWriter.writeArithmetic("not");
      }
    }
  }

  void compileSubroutineCall()
  {
    string name = tokenString;
    int nArgs = 0;
    loadNxtToken();

    if (tokenType == "symbol" && tokenString == "(")
    {
      myWriter.writePush("pointer", "0");
      nArgs = compileExpressionList() + 1;
      loadNxtToken(); //)
      myWriter.writeCall(currentClassName + "." + name, nArgs);
    }
    else if (tokenType == "symbol" && tokenString == ".")
    {
      string objName = name;
      loadNxtToken();

      name = tokenString;

      string type = myTable.TypeOf(objName);

      if (type == "int" || type == "boolean" || type == "string" || type == "void")
      {
        cout << "NO BUILT IN TYPE" << endl;
      }
      else if (type == "")
      {
        name = objName + "." + name;
      }
      else
      {
        // nArgs = 1;
        // myWriter.writePush(myTable.KindOf(name), to_string(myTable.IndexOf(name)));
        // name = myTable.TypeOf(objName) + "." + name;
      }

      loadNxtToken(); //(

      nArgs += compileExpressionList();

      loadNxtToken(); //)

      myWriter.writeCall(name, nArgs);
    }
  }
*/

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
    loadNxtToken(); //identifier
    loadNxtToken(); //[
    compileExpression();
    loadNxtToken(); //]
  }
  else if (tokenString == "(")
  {
    loadNxtToken(); //(
    compileExpression();
    loadNxtToken(); //)
  }
  // else if (myTokenizer.lookBehindType() == "symbol" && tokenType == "symbol")
  // {
  //   outputTuple = make_tuple("arithOperator", arithPreTable[tokenString], "");
  //   return outputTuple;
  // }
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
    outputTuple = make_tuple("bool", "-1", "");
    return outputTuple;
  }
  else if (tokenString == "false")
  {
    outputTuple = make_tuple("bool", "0", "");
    return outputTuple;
  }
}