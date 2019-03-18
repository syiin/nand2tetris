#include <iostream>
#include <string>
#include "Tokenizer.h"

using namespace std;
int main(int argc, char *argv[])
{
  Tokenizer myTokenizer(*(argv + 1));
  myTokenizer.printTokens();
  return 0;
}