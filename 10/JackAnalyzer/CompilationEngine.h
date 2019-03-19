#ifndef COMPILATIONENG_H
#define COMPILATIONENG_H

#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <experimental/filesystem>

using namespace std;
// namespace fs = std::filesystem;

class CompilationEngine
{
private:
  ofstream outputFile;
  map<string, string> inputDir;

public:
  CompilationEngine(string inputDirName, string outputFileName)
  {
    inputDir = loadDir(inputDirName);
    outputFile.open(outputFileName);
    compileClass();
  }

  void compileClass()
  {
  }

  void compileClassVarDec()
  {
  }

  void compileSubroutineDec()
  {
  }

  map<string, string> loadDir(string inputDirName)
  {
    map<string, string> fileMap;
    if (isDir(inputDirName))
    {
      fileMap = handleDir(inputDirName);
    }
    else
    {
      fileMap[getFileName(inputDirName)] = inputDirName;
    }
    return fileMap;
  }

  map<string, string> handleDir(string inputDirName)
  {
    map<string, string> outputMap;
    for (const auto &entry : experimental::filesystem::directory_iterator(inputDirName))
    {
      string filePath = entry.path();
      string fileName = getFileName(filePath);

      if (isDir(filePath))
      {
        outputMap[fileName] = filePath;
      };
    }
    return outputMap;
  }

  string getFileName(string fileString)
  {
    int start = fileString.find_last_of('/');
    int end = fileString.find('\0');
    return fileString.substr(start + 1, end);
  }

  bool isDir(string inputDirName)
  {
    int jackFileNameStart = inputDirName.find_last_of('.');
    int jackFileNameEnd = inputDirName.find('\0');
    string fileType = inputDirName.substr(jackFileNameStart, jackFileNameEnd);
    return (fileType == ".jack");
  }
};

#endif