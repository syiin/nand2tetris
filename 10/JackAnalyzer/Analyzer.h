#ifndef ANALYZER_H
#define ANALYZER_H

#include <fstream>
#include <experimental/filesystem>
#include "Tokenizer.h"
#include "CompilationEngine.h"

using namespace std;

class Analyzer
{
private:
  CompilationEngine compEngine;
  map<string, string> inputDir;
  string outputFileName;

public:
  Analyzer(string inputDirName)
  {
    inputDir = loadDir(inputDirName);
  }

  void createXML()
  {
    for (auto const &file : inputDir)
    {
      compEngine.setOutputFileName(file.first);
      compEngine.loadEngine(file.second);
    }
  }

  //DIRECTORY FUNCTIONS
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
    if (inputDirName.find_last_of('.') == string::npos)
    {
      return true;
    }
    int jackFileNameStart = inputDirName.find_last_of('.');
    int jackFileNameEnd = inputDirName.find('\0');
    string fileType = inputDirName.substr(jackFileNameStart, jackFileNameEnd);
    return (fileType == ".jack");
  }
};

#endif