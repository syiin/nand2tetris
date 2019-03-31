#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <string>

struct SymbolEntry
{
  string kind;
  string type;
  int idx;
};

class SymbolTable
{
private:
  map<string, SymbolEntry> classTable;
  map<string, SymbolEntry> subroutineTable;

public:
  SymbolTable()
  {
  }

  void startSubroutine()
  {
    subroutineTable.clear();
  }

  void define(string name, string kind, string type)
  {
    if (kind == "static" || kind == "field")
    {
      addToClassTable(name, kind, type);
    }
    else
    {
      addToSubroutineTable(name, kind, type);
    }
  }

  void addToClassTable(string name, string kind, string type)
  {
    int newIdx = VarCount(classTable, kind);
    SymbolEntry newEntry = createNewEntry(kind, type, newIdx);
    classTable[name] = newEntry;
  }

  void addToSubroutineTable(string name, string kind, string type)
  {
    int newIdx = VarCount(subroutineTable, kind);
    SymbolEntry newEntry = createNewEntry(kind, type, newIdx);
    subroutineTable[name] = newEntry;
  }

  int VarCount(map<string, SymbolEntry> tableToSearch, string kind)
  {
    int count = 0;
    for (auto const &symbol : tableToSearch)
    {
      if (symbol.second.kind == kind)
      {
        count++;
      }
    }
    return count;
  }

  string KindOf(string name)
  {
    if (checkIfContainsName(subroutineTable, name))
    {
      if (subroutineTable[name].kind == "var")
      {
        return "local";
      }
      return subroutineTable[name].kind;
    }
    else if (checkIfContainsName(classTable, name))
    {
      if (classTable[name].kind == "var")
      {
        return "local";
      }
      return classTable[name].kind;
    }
    return "NOT FOUND";
  }

  string TypeOf(string name)
  {
    if (checkIfContainsName(subroutineTable, name))
    {
      return subroutineTable[name].type;
    }
    else if (checkIfContainsName(classTable, name))
    {
      return classTable[name].type;
    }
    return "NOT FOUND";
  }

  int IndexOf(string name)
  {
    if (checkIfContainsName(subroutineTable, name))
    {
      return subroutineTable[name].idx;
    }
    else if (checkIfContainsName(classTable, name))
    {
      return classTable[name].idx;
    }
  }

  bool checkIfContainsName(map<string, SymbolEntry> tableToSearch, string name)
  {
    for (auto const &symbol : tableToSearch)
    {
      if (symbol.first == name)
      {
        return true;
      }
    }
    return false;
  }

  SymbolEntry createNewEntry(string kind, string type, int idx)
  {
    SymbolEntry newEntry;
    newEntry.kind = kind;
    newEntry.type = type;
    newEntry.idx = idx;
    return newEntry;
  }

  void printTable()
  {
    cout << "CLASS TABLE:" << endl
         << "NAME \t"
         << "TYPE \t"
         << "KIND \t"
         << "IDX \t" << endl;
    for (auto const &symbol : classTable)
    {
      cout << symbol.first << '\t'
           << symbol.second.type << '\t'
           << symbol.second.kind << '\t'
           << symbol.second.idx << '\t'
           << endl;
    }

    cout << "SUBROUTINE TABLE:" << endl
         << "NAME \t"
         << "TYPE \t"
         << "KIND \t"
         << "IDX \t" << endl;
    for (auto const &symbol : subroutineTable)
    {
      cout << symbol.first << '\t'
           << symbol.second.type << '\t'
           << symbol.second.kind << '\t'
           << symbol.second.idx << '\t'
           << endl;
    }
  }
};

#endif