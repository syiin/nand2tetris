#include <map>

#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "CompilationEngine.h"
#include "VMWriter.h"

class JackCompiler
{
private:
  CompilationEngine myEngine;
  map<string, string> inputDir;
  string currOutputFileName;

public:
  JackCompiler(string inputDirName)
  {
    inputDir = loadDir(inputDirName);
  }

  void compileVMCode()
  {
    for (auto const &file : inputDir)
    {
      cout << file.second << endl;
      myEngine.setOutputFileName(file.second);
      myEngine.loadEngine(file.second);
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
        int end = fileName.find_last_of('.');
        outputMap[fileName.substr(0, end)] = filePath;
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
