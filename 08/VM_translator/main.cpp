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
void translate(Parser myParser, CodeWriter myWriter);

int main(int argc, char *argv[])
{
  std::map<std::string, std::string> files;

  Parser myParser;
  CodeWriter myWriter(*(argv + 2));

  files = loadDir(*(argv + 1));

  myParser.loadTokens(files["Sys.vm"]);
  myWriter.setFileName("Sys.vm");

  while (myParser.hasMoreCommands())
  {
    std::cout << "C_Typ: \t" << myParser.commandType() << std::endl;
    std::cout << "Arg1: \t" << myParser.arg1() << std::endl;
    std::cout << "Arg2: \t" << myParser.arg2() << std::endl;

    if (myParser.commandType() == "C_ARITHMETIC")
    {
      myWriter.writeArithmetic(myParser.arg1());
    }
    else if (myParser.commandType() == "C_PUSH" || myParser.commandType() == "C_POP")
    {
      myWriter.writePushPop(myParser.commandType(),
                            myParser.arg1(),
                            std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_LABEL")
    {
      myWriter.writeLabel(myParser.arg1());
    }
    else if (myParser.commandType() == "C_IF")
    {
      myWriter.writeIf(myParser.arg1());
    }
    else if (myParser.commandType() == "C_GOTO")
    {
      myWriter.writeGoto(myParser.arg1());
    }
    else if (myParser.commandType() == "C_FUNCTION")
    {
      myWriter.writeFunction(myParser.arg1(), std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_RETURN")
    {
      myWriter.writeReturn();
    }
    else if (myParser.commandType() == "C_CALL")
    {
      myWriter.writeCall(myParser.arg1(), std::stoi(myParser.arg2()));
    };
    myParser.advance();
  };
  myParser.showTokens();

  files.erase("Sys.vm");

  for (auto const &file : files)
  {
    std::cout << "FIRST: " << file.first
              << "SECOND: " << file.second
              << std::endl;

    myParser.loadTokens(file.second);
    myWriter.setFileName(file.first);
    while (myParser.hasMoreCommands())
    {
      std::cout << "C_Typ: \t" << myParser.commandType() << std::endl;
      std::cout << "Arg1: \t" << myParser.arg1() << std::endl;
      std::cout << "Arg2: \t" << myParser.arg2() << std::endl;

      if (myParser.commandType() == "C_ARITHMETIC")
      {
        myWriter.writeArithmetic(myParser.arg1());
      }
      else if (myParser.commandType() == "C_PUSH" || myParser.commandType() == "C_POP")
      {
        myWriter.writePushPop(myParser.commandType(),
                              myParser.arg1(),
                              std::stoi(myParser.arg2()));
      }
      else if (myParser.commandType() == "C_LABEL")
      {
        myWriter.writeLabel(myParser.arg1());
      }
      else if (myParser.commandType() == "C_IF")
      {
        myWriter.writeIf(myParser.arg1());
      }
      else if (myParser.commandType() == "C_GOTO")
      {
        myWriter.writeGoto(myParser.arg1());
      }
      else if (myParser.commandType() == "C_FUNCTION")
      {
        myWriter.writeFunction(myParser.arg1(), std::stoi(myParser.arg2()));
      }
      else if (myParser.commandType() == "C_RETURN")
      {
        myWriter.writeReturn();
      }
      else if (myParser.commandType() == "C_CALL")
      {
        myWriter.writeCall(myParser.arg1(), std::stoi(myParser.arg2()));
      };
      myParser.advance();
    };
    myParser.showTokens();
  }
}

void translate(Parser myParser, CodeWriter myWriter)
{
  while (myParser.hasMoreCommands())
  {
    std::cout << "C_Typ: \t" << myParser.commandType() << std::endl;
    std::cout << "Arg1: \t" << myParser.arg1() << std::endl;
    std::cout << "Arg2: \t" << myParser.arg2() << std::endl;

    if (myParser.commandType() == "C_ARITHMETIC")
    {
      myWriter.writeArithmetic(myParser.arg1());
    }
    else if (myParser.commandType() == "C_PUSH" || myParser.commandType() == "C_POP")
    {
      myWriter.writePushPop(myParser.commandType(),
                            myParser.arg1(),
                            std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_LABEL")
    {
      myWriter.writeLabel(myParser.arg1());
    }
    else if (myParser.commandType() == "C_IF")
    {
      myWriter.writeIf(myParser.arg1());
    }
    else if (myParser.commandType() == "C_GOTO")
    {
      myWriter.writeGoto(myParser.arg1());
    }
    else if (myParser.commandType() == "C_FUNCTION")
    {
      myWriter.writeFunction(myParser.arg1(), std::stoi(myParser.arg2()));
    }
    else if (myParser.commandType() == "C_RETURN")
    {
      myWriter.writeReturn();
    }
    else if (myParser.commandType() == "C_CALL")
    {
      myWriter.writeCall(myParser.arg1(), std::stoi(myParser.arg2()));
    };
    myParser.advance();
  };
}

std::map<std::string, std::string> loadDir(std::string inputDir)
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