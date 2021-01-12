/*
 * CSE 109
 * Athicha Srivirote
 * ats219
 * Program Description: Lexer class has 2 different methods
 *                      nextChar returns the next character
 *                      nextToken returns the next token that contains 
 *                                type, lexeme, line, and position               * 
 *                      
 * Compiler 
 */
#include "lexer.h"

Lexer::Lexer(istream& in)
  :stream(in), 
  line(1), 
  pos(0) {
  ch = nextChar();
}


char Lexer::nextChar() {
 if(!stream.get(ch)) {
   return '#';
 }
 pos++;
 if (ch == '\n') {
   line++;
   pos = 0;
   return ' ';
 } 
 return ch;
}

Token Lexer::nextToken() {
  int saveLine;
  int savePos;
  string lexeme = "";
  int type;
  while (isspace(ch)) {
    ch = nextChar();
  }

  saveLine = line;
  savePos = pos;
  if (isalpha(ch)) {
    const int size = 7;
    int types[size] = {Token::IF, Token::ELSE, Token::WHILE,
                       Token::FUNCTION, Token::VAR, Token::PRINTF, Token::RETURN};
    string keywords[size] = {"if" ,"else", "while",
                             "function", "var", "printf", "return"};

    lexeme += ch;
    ch = nextChar();
    while (isalpha(ch) || isdigit(ch)) {
      lexeme += ch;
      ch = nextChar();
    }

    int ok = 0;
    for ( int i = 0; i < size; i++) {
      if (lexeme == keywords[i]) {
        type = types[i];
	ok = 1;
	break;
      }
    } 
    if (ok == 0)
      type = Token::IDENT;

    Token token = Token(type, lexeme, saveLine, savePos); 
    return token;   

  } else if (isdigit(ch)) {
      while (isdigit(ch)) {
        lexeme += ch;
        ch = nextChar();
     }
     type = Token::INTLIT;
     Token token = Token(type, lexeme, saveLine, savePos); 
     return token;

    } else if (ch == '"') {
        ch = nextChar();
        while (ch != '"') { 
	  lexeme += ch;
	  ch = nextChar();
	} 
	  type = Token::STRINGLIT;
          Token token = Token(type, lexeme, saveLine, savePos);
	  ch = nextChar();
	  return token;


    // '#' indicates the end of the file 
    } else if (ch == '#') {
        lexeme = ch;
        type = Token::ENDOFFILE;
        Token token = Token(type, lexeme, saveLine, savePos);
	return token;
	
    } else {
        switch (ch) {
        case '+':
	  lexeme = ch;
	  type = Token::PLUS;
          break;

	case '-':
	  lexeme = ch;
	  type = Token::MINUS;
	  break;

	case '*':
	  lexeme = ch;
	  type = Token::TIMES;
	  break;

	case '/':
          lexeme = ch;
	  type = Token::DIVIDE;
	  break;

	case '=':
	  lexeme = ch;
	  ch = nextChar();
          if (ch == '=') {
            lexeme += ch;
	    type = Token::EQ;
	  } else {
	      type = Token::ASSIGN;
	      Token token = Token(type, lexeme, saveLine, savePos);
	      return token;
	  }
	  break;
         
        case '!':
	  lexeme = ch;
          ch = nextChar();
	  if (ch == '=') {
            lexeme += ch;
	    type = Token::NE;
	  } else {
              lexeme = ch;
              type = Token::ERROR;
              cerr << "ERROR: invalid token: " << lexeme << " at line: "
                   << saveLine << " postion: " << savePos << endl;
              Token token = Token(type, lexeme, saveLine, savePos);
              return token;
              exit(1);
	      } 
	  break;

	case '<':
          lexeme = ch;
	  ch = nextChar();
	  if (ch == '=') {
	    lexeme += ch;
	    type = Token::LE;
	  } else {
              type = Token::LT;
              Token token = Token(type, lexeme, saveLine, savePos);
	      return token;
	  }
	  break;
           
        case '>':
          lexeme = ch;
          ch = nextChar();
          if (ch == '=') {
            lexeme += ch;
            type = Token::GE;
          } else {
              type = Token::GT;
	      Token token = Token(type, lexeme, saveLine, savePos);
	      return token;
          }
          break;       
         
        case '&':
          lexeme = ch;
          ch = nextChar();
          if (ch == '&') {
            lexeme += ch;
            type = Token::AND;
          } else {
              lexeme = ch;
              type = Token::ERROR;
              cerr << "ERROR: invalid token: " << lexeme << " at line: "
                   << saveLine << " postion: " << savePos << endl;
              Token token = Token(type, lexeme, saveLine, savePos);
	      return token;
              exit(1);
          }
          break; 
    	   
	case '|':
          lexeme = ch;
          ch = nextChar();
          if (ch == '|') {
            lexeme += ch;
            type = Token::OR;
          } else {
              lexeme = ch;
              type = Token::ERROR;
              cerr << "ERROR: invalid token: " << lexeme << " at line: "
	           << saveLine << " postion: " << savePos << endl;
              Token token = Token(type, lexeme, saveLine, savePos);
              return token;
              exit(1);
          }
          break; 
       
        case '(':
          lexeme = ch;
          type = Token::LPAREN;
          break; 

        case ')':
          lexeme = ch;
          type = Token::RPAREN;
          break;
 
        case '{':
	  lexeme = ch;
          type = Token::LBRACE;
          break;

        case '}':
          lexeme = ch;
          type = Token::RBRACE;
          break;

        case ',':
          lexeme = ch;
          type = Token::COMMA;
          break;
	  
	case ';':
          lexeme = ch;
          type = Token::SEMICOLON;
          break;

        default :
          lexeme = ch;
          type = Token::ERROR;
          cerr << "ERROR: invalid token: " << lexeme << " at line: "
               << saveLine << " postion: " << savePos << endl;
          Token token = Token(type, lexeme, saveLine, savePos);
          return token;
          exit(1);
    }
       Token token = Token(type, lexeme, saveLine, savePos);
       ch = nextChar();
       return token;
  }
}
