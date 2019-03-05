#ifndef CODE_H
#define CODE_H

#include <fstream>
#include <map>
#include <string>

class CodeWriter
{

private:
  std::map<std::string, std::string> aTable;
  std::string fileName = "";
  std::ofstream myfile;

public:
  CodeWriter(std::string outputName)
  {
    fileName = outputName;
    myfile.open(fileName);
  };

  void writeArithmetic(std::string theCommand)
  {
    std::string outputString = "";
    if (theCommand == "add")
    {
      outputString = "@SP \n A = M \n  D = M \n A = A - 1 \n D = D + M \n @SP \n M = M - 1 \n A = M \n M = D";
    };
    outputString = outputString + theCommand;

    myfile << outputString;
  };

  void writePushPop(std::string theCommand, std::string segment, int index)
  {
    std::string outputString = "";
    outputString = outputString + theCommand;

    myfile << outputString;
  };

  void close()
  {
    myfile.close();
  }
};

#endif