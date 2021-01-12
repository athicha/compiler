/*
 * CSE109
 * Athicha Srivirote 
 * ats219
 * Program Description: SymbolTable using map to map out string to interger
 *                      key and also using stack to store keys value 
 */
#include <sstream>
#include "symboltable.h"
#include "stack.cc"

SymbolTable::SymbolTable() 
  : symbols(), size(0) {
}

SymbolTable::~SymbolTable() {}

void SymbolTable::enterScope() {
  size ++;
  symbols.push(new map<string,int>());
}

void SymbolTable::exitScope() {
  map<string,int> *h = symbols.peek();
  delete h;

  size --;
  symbols.pop();
}

int SymbolTable::addSymbol(string sym) {
  map<string,int> *h = symbols.peek();

  if (h->count(sym) > 0)
    return 0;

  (*h)[sym] = size;
  return 1;
}

string SymbolTable::getUniqueSymbol(string sym) {
  for (int i = size-1; i >= 0; i --) {
    map<string,int> *h = symbols[i];
    if (h->count(sym)) {
      stringstream out;
      out << sym << "$" << (*h)[sym];
      return out.str();
    }
  }
  return "";
}
