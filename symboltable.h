/* CSE109 
 * Athicha Srivirote
 * ats219
 * Program Discription : SymbolTable.h contains fields and methods for
 *                       SymbolTable.cc
 */ 
#include <string>
#include <map>
using namespace std;

#include "stack.h"

class SymbolTable {

public:

  SymbolTable();
  ~SymbolTable();

  void enterScope();
  void exitScope();
  int addSymbol(string sym);
  string getUniqueSymbol(string sym);

private:
  Stack<map<string,int> *> symbols;
  int size;
};
