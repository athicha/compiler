// this is a main program just like lextest 
#include "parser.h"
#include <iostream>
#include <fstream>

using namespace std;

void processFile(istream& in) {
  Lexer lexer(in);
  Parser parser(lexer, cout);
  Parser::TreeNode* program = parser.compilationunit(); 
  parser.genasm(program); // write out the tree 
}

int main(int argc, char **argv) {
  ifstream in;
  
  if (argc > 1) {
      in.open(argv[1]);
      processFile(in);
      in.close();
  } else {
    processFile(cin);
  }
  return 0;
  
}
