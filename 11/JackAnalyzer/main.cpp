#include <iostream>
#include <string>
#include "JackCompiler.h"
#include "SymbolTable.h"

using namespace std;

int main(int argc, char *argv[])
{
  JackCompiler myCompiler(*(argv + 1));
  myCompiler.compileVMCode();
  return 0;
}

//g++ -std=c++17 main.cpp -lstdc++fs to use <experimental/filesystem> on Ubuntu machine