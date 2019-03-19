#include <iostream>
#include <string>
#include "Analyzer.h"

using namespace std;
int main(int argc, char *argv[])
{
  Analyzer myAnalyzer(*(argv + 1));
  myAnalyzer.createXML();
  return 0;
}

//g++ -std=c++17 main.cpp -lstdc++fs to use <experimental/filesystem> on Ubuntu machine