#ifndef VMWRITER_H
#define VMWRITER_H

#include <string>

class VMWriter
{
private:
  string outputFileName;
  ofstream outputFile;
  vector<string> ARITH_SYMBOLS = {"add", "neg", "*", "/", ">", "<"};

public:
  void startWriter(string fileName)
  {
    outputFileName = fileName;
    int end = fileName.find_last_of('.');
    outputFileName = fileName.substr(0, end);
    outputFile.open(outputFileName + ".vm");

    cout << outputFileName + ".vm" << endl;
  }

  void writePush(string segment, string idx)
  {
    outputFile << "push " << segment << " " << idx << endl;
  }

  void writePop(string segment, int idx)
  {
    outputFile << "pop " << segment << " " << to_string(idx) << endl;
  }

  void writeArithmetic(string command)
  {
    if (isArithSymbol(command))
    {
      outputFile << command << endl;
    }
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
    outputFile << "return \n\n";
  }

  void writeComment(string comment)
  {
    outputFile << "\tCOMMENT:\t" << comment << endl;
  }

  void close()
  {
    outputFile.close();
  }

  bool isArithSymbol(const string &line)
  {
    if (find(ARITH_SYMBOLS.begin(), ARITH_SYMBOLS.end(), line) != ARITH_SYMBOLS.end())
      return true;
    else
      return false;
  }
};

#endif