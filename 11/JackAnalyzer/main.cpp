#include <iostream>
#include <string>
#include "Analyzer.h"
#include "SymbolTable.h"

using namespace std;
int main(int argc, char *argv[])
{
  // Analyzer myAnalyzer(*(argv + 1));
  SymbolTable mySymbolTable;

  cout << "FIRST DEF: " << endl;
  mySymbolTable.define("first", "static", "int");
  mySymbolTable.define("second", "static", "int");
  mySymbolTable.define("first", "var", "int");
  mySymbolTable.define("second", "var", "int");

  mySymbolTable.printTable();

  cout << "START SUB: " << endl;
  mySymbolTable.startSubroutine();
  mySymbolTable.printTable();

  cout << "SECOND DEF: " << endl;
  mySymbolTable.define("third", "var", "int");
  mySymbolTable.define("fourth", "var", "int");

  cout << "KIND OF THIRD :" << endl;
  cout << mySymbolTable.KindOf("third") << mySymbolTable.TypeOf("third") << to_string(mySymbolTable.IndexOf("third")) << endl;

  cout << "KIND OF FIRST :" << endl;
  cout << mySymbolTable.KindOf("first") << mySymbolTable.TypeOf("first") << to_string(mySymbolTable.IndexOf("first")) << endl;

  return 0;
}

//g++ -std=c++17 main.cpp -lstdc++fs to use <experimental/filesystem> on Ubuntu machine