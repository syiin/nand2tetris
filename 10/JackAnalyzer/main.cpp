#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "CompilationEngine.h"

using namespace std;
int main(int argc, char *argv[])
{
  Tokenizer myTokenizer(*(argv + 1));
  CompilationEngine myCompilationEng(*(argv + 1), "output.xml");

  myTokenizer.printTokens();
  return 0;
}

//g++ -std=c++17 main.cpp -lstdc++fs to use <experimental/filesystem> on Ubuntu machine