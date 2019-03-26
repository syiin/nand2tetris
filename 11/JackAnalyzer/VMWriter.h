#ifndef VMWRITER_H
#define VMWRITER_H

#include <string>

class VMWriter
{
private:
  string outputFileName;
  ofstream outputFile;

public:
  void startWriter(string fileName)
  {
    outputFileName = fileName;
    int end = fileName.find_last_of('.');
    outputFileName = fileName.substr(0, end);
    outputFile.open(outputFileName + ".vm");

    cout << outputFileName + ".vm" << endl;
    }

  void writePush(string segment, int idx)
  {
    outputFile << "push " << segment << " " << to_string(idx) << endl;
  }

  void writePop(string segment, int idx)
  {
    outputFile << "pop " << segment << " " << to_string(idx) << endl;
  }

  void writeArithmetic(string command)
  {
    outputFile << command << endl;
  }

  void writeLabel(string label)
  {
    outputFile << "label " << label << endl;
  }

  void writeGoTo(string label)
  {
    outputFile << "goto " << label << endl;
  }

  void writeIf(string label)
  {
    outputFile << "if-goto " << label << endl;
  }

  void writeCall(string name, int nArgs)
  {
    outputFile << "call " << name << " " << to_string(nArgs) << endl;
  }

  void writeFunction(string name, int nLocals)
  {
    outputFile << "function " << name << " " << to_string(nLocals) << endl;
  }

  void writeReturn()
  {
    outputFile << "return";
  }

  void close()
  {
    outputFile.close();
  }
};

#endif