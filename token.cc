/*
 * CSE 109
 * Athicha Srivirote
 * ats219
 * Program Description: Token class has getters for type, lexeme, line, and pos
 *                      Token represents a token that will be returned 
 * Compiler 
 */
#include "token.h"

// type getter
int Token:: getType() {
  return type;
}

// lexeme getter
string Token:: getLexeme() {
  return lexeme;
}

// line getter
int Token::getLine() {
  return line;
}

// pos getter
int Token::getPos() {
  return pos;
}

// This is the only constructor of the class
// It will initialize type, lexeme, line, and pos
Token::Token(int intValue, string input, int lineNumber, int position) {
  type = intValue;
  lexeme = input;
  line = lineNumber;
  pos = position;

}

Token::~Token() {
}

