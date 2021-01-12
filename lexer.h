/*
 * CSE 109
 * Athicha Srivirote
 * ats219
 * Program Description: Lexer.h contains all fields of Lexer class
 *                      
 * Compiler
 */
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <istream>
#include <iostream>
#include <stdlib.h>
#include "token.h"

using namespace std;

class Lexer {

public:
  Lexer(istream& in);
  Token nextToken();
private:
  istream& stream;
  int line; 
  int pos;
  char ch;
  char nextChar();


};

