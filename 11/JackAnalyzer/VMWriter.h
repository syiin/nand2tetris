#ifndef VMWRITER_H
#define VMWRITER_H

#include <string>

class VMWriter
{
private:
  string outputFileName;
  ofstream outputFile;
  vector<string> ARITH_SYMBOLS = {"add", "neg", "not", "call Math.multiply 2", "call Math.divide 2", "gt", "lt", "sub", "and", "or", "eq"};

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

  void writePop(string segment, string idx)
  {
    outputFile << "pop " << segment << " " << idx << endl;
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

  void writeCall(string name, string nArgs)
  {
    outputFile << "call " << name << " " << nArgs << endl;
  }

  void writeFunction(string name, int nLocals)
  {
    outputFile << "function " << name << " " << to_string(nLocals) << endl;
  }

  void writeReturn()
  {
    outputFile << "return" << endl;
  }

  void writeComment(string comment)
  {
    outputFile << "\tCOMMENT:\t" << comment << endl;
  }

  void writeBreak()
  {
    outputFile << endl;
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