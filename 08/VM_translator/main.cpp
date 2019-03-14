#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <bitset>
#include <cstring>
#include <vector>
#include <map>

#include "Parser.h"
#include "CodeWriter.h"

std::map<std::string, std::string> loadDir(std::string inputDir);
void translateCode(Parser *myParser, CodeWriter *myWriter);

int main(int argc, char *argv[])
{
  std::map<std::string, std::string> files = loadDir(*(argv + 1));
  Parser myParser;
  CodeWriter myWriter(*(argv + 2));

  myParser.loadTokens(files["Sys.vm"]);
  myWriter.setFileName("Sys.vm");
  translateCode(&myParser, &myWriter); //make sure Sys.vm is loaded and written first

  files.erase("Sys.vm"); //prepare to load the other files in the directory
  for (auto const &file : files)
  {
    myParser.loadTokens(file.second);
    myWriter.setFileName(file.first);
    translateCode(&myParser, &myWriter);
    myParser.showTokens();
  }
}

void translateCode(Parser *myParser, CodeWriter *myWriter)
//Gets tokens line by line from the Parser and feeds it into the CodeWriter
{
  while (myParser->hasMoreCommands())
  {
    std::cout << "C_Typ: \t" << myParser->commandType() << std::endl;
    std::cout << "Arg1: \t" << myParser->arg1() << std::endl;
    std::cout << "Arg2: \t" << myParser->arg2() << std::endl;

    if (myParser->commandType() == "C_ARITHMETIC")
    {
      myWriter->writeArithmetic(myParser->arg1());
    }
    else if (myParser->commandType() == "C_PUSH" || myParser->commandType() == "C_POP")
    {
      myWriter->writePushPop(myParser->commandType(),
                             myParser->arg1(),
                             std::stoi(myParser->arg2()));
    }
    else if (myParser->commandType() == "C_LABEL")
    {
      myWriter->writeLabel(myParser->arg1());
    }
    else if (myParser->commandType() == "C_IF")
    {
      myWriter->writeIf(myParser->arg1());
    }
    else if (myParser->commandType() == "C_GOTO")
    {
      myWriter->writeGoto(myParser->arg1());
    }
    else if (myParser->commandType() == "C_FUNCTION")
    {
      myWriter->writeFunction(myParser->arg1(), std::stoi(myParser->arg2()));
    }
    else if (myParser->commandType() == "C_RETURN")
    {
      myWriter->writeReturn();
    }
    else if (myParser->commandType() == "C_CALL")
    {
      myWriter->writeCall(myParser->arg1(), std::stoi(myParser->arg2()));
    };
    myParser->advance();
  };
}

std::map<std::string, std::string> loadDir(std::string inputDir)
//Takes in a directory string and returns a map where the key is filename and value is the address
{
  std::map<std::string, std::string> fileMap;

  //find the rest
  for (const auto &entry : fs::directory_iterator(inputDir))
  {
    std::string filePath = entry.path();

    int vmFileNameStart = filePath.find_last_of('.');
    int vmFileNameEnd = filePath.find('\0');

    int start = filePath.find_last_of('/');
    int end = filePath.find('\0');

    std::string fileType = filePath.substr(vmFileNameStart, vmFileNameEnd);
    std::string fileName = filePath.substr(start + 1, end);

    if (fileType == ".vm")
    {
      fileMap[fileName] = filePath;
    };
  }

  return fileMap;
}