/*
 * CSE 109
 * Athicha Srivirote
 * ats219
 * Program Description: Parser consists of functions that return a pointer to TreeNode that will represent 
 *                      the grammar 
 *
 */

#include "parser.h"
#include <cstring>

const string Parser::ops[] = {"ADD", "SUB", "MULT", "DIV",
                      "ISEQ", "ISNE", "ISLT", "ISLE", "ISGT", "ISGE",
                      "AND", "OR",
                      "PUSHL", "PUSHV", "STORE",
                      "JUMP", "JUMPF", "JUMPT", "CALL", "RET",
                      "PRINTF",
                      "LABEL", "SEQ",
		      "FUNC",
		      "PAREM1", "PAREM2", "PAREM3", "PAREM4", "PAREM5"
		      };

Parser::Parser(Lexer& lexerx, ostream& outx)
  : nfmts(0),
    lexer(lexerx),
    token(lexer.nextToken()),
    out(outx),
    lindex(1), 
    tindex(1) {
}


Parser::~Parser() {
}
void Parser::emitComparison(Parser::TreeNode* node) {
  cout << "pop rbx" << endl;
  cout << "pop rax" << endl;
  cout << "cmp rax,rbx" << endl;
  string j1 = makeLabel();
  string j2 = makeLabel();
  switch (node->op) {
    case ISEQ:
      cout << "je " << j1 << endl;
      break;
    case ISNE:
      cout << "jne " << j1 << endl;
      break;
    case ISLT:
      cout << "jl " << j1 << endl;
      break;
    case ISLE:
      cout << "jle " << j1 << endl;
      break;
    case ISGT:
      cout << "jg " << j1 << endl;
      break;
    case ISGE: 
      cout << "jge " << j1 << endl;
      break;
    default: ;
  }
  cout << "mov rax,0" << endl;
  cout << "jmp " << j2 << endl;
  cout << j1 << ":" << endl;
  cout << "mov rax,1" << endl;
  cout << j2 << ":" << endl;
  cout << "push rax" << endl;
}
void Parser::emit(Parser::TreeNode* node) {
  int nparams;
  string fmt;

  switch (node->op) {
    case ADD:
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "add rax,rbx" << endl;
      cout << "push rax" << endl;
      break;
    case SUB:
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "sub rax,rbx" << endl;
      cout << "push rax" << endl;
      break;
    case AND:
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "and rax,rbx" << endl;
      cout << "push rax" << endl;
      break;
    case OR:
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "or rax,rbx" << endl;
      cout << "push rax" << endl;
      break;
    case MULT:
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "imul rbx" << endl;
      cout << "push rax" << endl;
      break;
    case DIV:
      cout << "mov rdx,0" << endl;
      cout << "pop rbx" << endl;
      cout << "pop rax" << endl;
      cout << "idiv rbx" << endl;
      cout << "push rax" << endl;
      break;
    case LABEL:
      cout << node->val << ":" << endl;
      break;
    case ISEQ:
    case ISNE:
    case ISLT:
    case ISLE: 
    case ISGT:
    case ISGE:
      emitComparison(node);
      break;
    case JUMP:
      cout << "jmp " << node->val << endl;
      break;
    case JUMPF:
      cout << "pop rax" << endl;
      cout << "cmp rax,0" << endl;
      cout << "je " << node->val << endl;
      break;
    case JUMPT:
      cout << "pop rax" << endl;
      cout << "cmp rax,0" << endl;
      cout << "jne " << node->val << endl;
      break;
    case PUSHV:
      cout << "push qword[" << node->val << "]" << endl;
      break;
    case PUSHL:
      cout << "mov rax," << node->val << endl;
      cout << "push rax" << endl;
      break;
    case STORE:
      cout << "pop qword[" << node->val << "]" << endl;
      break;
    case PRINTF:
      fmt = node->val;
      nparams = fmt.at(0) - '0';
      fmt = "`" + fmt.substr(1) + "`";
      fmts[nfmts++] = fmt; 
      cout << "mov rdi,fmt" << itos(nfmts) << endl;
      if (nparams == 5) {
        cout << "pop r9" << endl;
	--nparams;
      }
      if (nparams == 4) {
        cout << "pop r8" << endl;
	--nparams;
      }
      if (nparams == 3) {
        cout << "pop rcx" << endl;
        --nparams;
      } 
      if (nparams == 2) {
        cout << "pop rdx" << endl;
	--nparams;
      }
      if (nparams == 1) {
        cout << "pop rsi" << endl;
      } 
      cout << "mov rax,0" << endl;
      cout << "push rbp" << endl;
      cout << "call printf" << endl;
      cout << "pop rbp" << endl;
      break;
    case CALL:
      cout << "call " << node->val << endl;
      cout << "push rax" << endl;
      break;
    case FUNC:
      currentFunc = node->val;
      cout << currentFunc << ":" << endl;
      if (currentFunc != "main") {
        cout << "pop r15" << endl;
      }
      break;
    case RET:
      cout << "pop rax" << endl;
      if (currentFunc != "main") {
        cout << "push r15" << endl;
      }
      cout << "ret" << endl;
      break;
    default: ;
  }
}

void Parser::vardefs(TreeNode* node, set<string>* vars) {
  if (node->leftChild != NULL) {
    vardefs(node->leftChild, vars);
  }
  if (node->rightChild != NULL) {
    vardefs(node->rightChild, vars);
  }
  if (node->op == PUSHV || node->op == STORE) {
    vars->insert(node->val);
  }
}
void Parser::geninst(TreeNode *node) {
  if (node->leftChild != NULL) {
    geninst(node->leftChild);
  }
  if (node->rightChild != NULL) {
    geninst(node->rightChild);
  }
  emit(node);
}
void Parser::genasm(TreeNode *node) {
  cout << "global main" << endl;
  cout << "extern printf" << endl;
  cout << "  segment .bss" << endl;
  set<string> vars;
  vardefs(node, &vars);
  for (set<string>::iterator it = vars.begin(); it != vars.end(); it++) {
    cout << *it << " resq 1" << endl;
  }

  cout << "section .data" << endl;
  cout << "fmt: db '%ld ', 0" << endl;
  cout << "endl: db 10, 0" << endl;
  cout << "  section .text" << endl;
  geninst(node);
  cout << "  section .data" << endl;
  int i;
  for (i = 0; i < nfmts ; i++) {
    cout << "fmt" << (i+1) << ": db " << fmts[i] << ", 0" << endl;
  }
}

void Parser::error(string message) {
  cerr << message << " Found " << token.getLexeme() << " at line " << token.getLine() << " position " << token.getPos() << endl;
  exit(1);
}

void Parser::check(int tokenType, string message) {
  if (token.getType() != tokenType)
    error(message);
}

Parser::TreeNode* Parser::funcall(string functionName) {
  Parser::TreeNode* call = new TreeNode(CALL, functionName);
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  Parser::TreeNode* node;
  if (token.getType() == Token::RPAREN) {
    return call;
  } 
  node =  expression();
  while (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    Parser::TreeNode* node2 = expression();
    node = new Parser::TreeNode(SEQ, node, node2);
  } 
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  return node = new Parser::TreeNode(SEQ, node, call); 
  
}

Parser::TreeNode* Parser::factor() {
  int tokenType = token.getType();
  string val, val_unique;
  Parser::TreeNode* node;
  switch (tokenType) { 
    case Token::LPAREN :
      token = lexer.nextToken();
      node = expression();
      check(Token::RPAREN, "Expecting )"); 
      token = lexer.nextToken();
      return node;

    case Token::INTLIT :
      val = token.getLexeme();
      token = lexer.nextToken();
      return new Parser::TreeNode(Parser::PUSHL, val);

    case Token::IDENT :
      val = token.getLexeme();
      token = lexer.nextToken();
      if (token.getType() == Token::LPAREN) {
        return funcall(val);
      }
      val_unique = symbolTable.getUniqueSymbol(val);
      if (val_unique == "") {
        error("Undefined variable " + val);
      }
      return new Parser::TreeNode(Parser::PUSHV, val_unique);
    default :
      error("Expecting a factor");
  }
  return NULL;
}

Parser::TreeNode* Parser::term() {
  Parser::TreeNode* node =  factor();
  while (token.getType() == Token::TIMES || token.getType() == Token::DIVIDE) {
    int tokenType  = token.getType(); 
    token = lexer.nextToken();
    Parser::TreeNode* node2 =  factor();
    if (tokenType == Token::TIMES) {
      node = new Parser::TreeNode(MULT, node, node2);
    } else {
        node = new Parser::TreeNode(DIV, node, node2);
    }
  }
  return node;
}

Parser::TreeNode* Parser::expression() {
  Parser::TreeNode* node =  term();
  while (token.getType() == Token::PLUS || token.getType() == Token::MINUS) {
    int tokenType  = token.getType(); 
    token = lexer.nextToken();
    Parser::TreeNode* node2 =  term();
    if (tokenType == Token::PLUS) {
      node = new Parser::TreeNode(ADD, node, node2);
    } else {
        node = new Parser::TreeNode(SUB, node, node2);
    }
  }
  return node;
}

Parser::TreeNode* Parser::relationalExpression() {
  Parser::TreeNode* node1 = expression();
  Operation op = LABEL; // placeholder 
  switch (token.getType()) {
    case Token::EQ :
      op = ISEQ; break; 
    case Token::LT :
      op = ISLT; break;
    case Token::LE :
      op = ISLE; break;
    case Token:: GT :
      op = ISGT; break;
    case Token::GE :
      op = ISGE; break;
    case Token::NE :
      op = ISNE; break;
    default :
      error("Expecting relational expression"); 
  }
  token = lexer.nextToken();
  Parser::TreeNode* node2 = expression();
  return new Parser::TreeNode(op, node1, node2);
}

Parser::TreeNode* Parser::logicalExpression() {
  Parser::TreeNode* node =  relationalExpression();
  while (token.getType() == Token::OR || token.getType() == Token::AND) {
    int tokenType  = token.getType(); 
    token = lexer.nextToken();
    Parser::TreeNode* node2 =  relationalExpression();
    if (tokenType == Token::OR) {
      node = new Parser::TreeNode(OR, node, node2);
    } else {
        node = new Parser::TreeNode(AND, node, node2);
    }
  }
  return node;
}

Parser::TreeNode* Parser::vardefStatement() {
  check(Token::VAR, "Expecting VAR");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting IDENT");
  string val = token.getLexeme();
  token = lexer.nextToken();
  if (symbolTable.addSymbol(val) == 0) {
    error("Redefining " + val + " in the same scope!");
  }
  while (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    check(Token::IDENT, "Expecting IDENT");
    val = token.getLexeme();
    token = lexer.nextToken();
    if (symbolTable.addSymbol(val) == 0) {
      error("Redefining " + val + " in the same scope!");
    }
  }
  check(Token::SEMICOLON, "Expecting SEMICOLON");
  token = lexer.nextToken();
  return new Parser::TreeNode(SEQ);
}

Parser::TreeNode* Parser::assignStatement() {
  check(Token::IDENT, "Expecting IDENT");
  string val = token.getLexeme();
  string val_unique = symbolTable.getUniqueSymbol(val);

  if (val_unique == "") {
    error("Undefined variable " + val);
  }
  Parser::TreeNode* stro = new Parser::TreeNode(STORE, val_unique);
  token = lexer.nextToken();
  check(Token::ASSIGN, "Expecting =");
  token = lexer.nextToken();
  Parser::TreeNode* expr = expression();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  return new Parser::TreeNode(SEQ, expr, stro);
}

Parser::TreeNode* Parser::printfStatement() {
  TreeNode* paramList = NULL;
  int nparams = 0;
  check(Token::PRINTF, "Expecting printf");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  check(Token::STRINGLIT, "Expecting string literal");
  string formatString = token.getLexeme();
  token = lexer.nextToken();
  if (token.getType() == Token::COMMA) {
    token = lexer.nextToken();
    paramList = expression();
    ++nparams;
    while (token.getType() == Token::COMMA) {
      token = lexer.nextToken();
      paramList = new TreeNode(SEQ, paramList, expression());
      ++nparams;
    }
  }  
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  check(Token::SEMICOLON, "Expecting ;");
  token = lexer.nextToken();
  TreeNode* printStatement =
    new TreeNode(SEQ, paramList, new TreeNode(PRINTF, itos(nparams) + formatString));
  return printStatement;
}

Parser::TreeNode* Parser::whileStatement() {
  check(Token::WHILE, "Expecting while statement");
  token = lexer.nextToken();
  string L1 = makeLabel();
  Parser::TreeNode* node = new Parser::TreeNode(LABEL, L1);
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  node = new Parser::TreeNode(SEQ, node, logicalExpression());
  string L2 = makeLabel();
  node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(JUMPF, L2));
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  node = new Parser::TreeNode(SEQ, node, block());
  node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(JUMP, L1));
  node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(LABEL, L2));
  return node;
}

Parser::TreeNode* Parser::ifStatement() {
  check(Token::IF, "Expecting if statement");
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  Parser::TreeNode* node = logicalExpression();
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  string L1 = makeLabel();
  node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(JUMPF, L1));
  node = new Parser::TreeNode(SEQ, node, block());
  if (token.getType() == Token::ELSE) {
    token = lexer.nextToken();
    string L2 = makeLabel();
    node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(JUMP, L2));
    node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(LABEL, L1));
    node = new Parser::TreeNode(SEQ, node, block());
    node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(LABEL, L2));
  } else {
    node = new Parser::TreeNode(SEQ, node, new Parser::TreeNode(LABEL, L1));
  }
  return node;
}

Parser::TreeNode* Parser::returnStatement() {
  check(Token::RETURN, "Expecting RETURN");
  Parser::TreeNode* ret = new Parser::TreeNode(RET);
  token = lexer.nextToken();
  Parser::TreeNode* node = expression();
  check(Token::SEMICOLON, "Expecting SEMICOLON");
  token = lexer.nextToken();
  return new Parser::TreeNode(SEQ, node, ret); 
}

Parser::TreeNode* Parser::statement() {
  switch (token.getType()) {
   case Token::PRINTF :
     return printfStatement();
   case Token::IDENT :
     return assignStatement();
   case Token::IF :
     return ifStatement();
   case Token::WHILE :
     return whileStatement();
   case Token::RETURN :
     return returnStatement();
   case Token::VAR :
     return vardefStatement();
   default :
     error("Expecting a statement");
  }
  return NULL;
}


Parser::TreeNode* Parser::parameterdefs() {
  Parser::TreeNode* node = new Parser::TreeNode(SEQ);
  string val;
  while (token.getType() != Token::RPAREN) {
    check(Token::IDENT, "Expecting IDENT");
    val = token.getLexeme();
    token = lexer.nextToken();
    symbolTable.addSymbol(val);
    node = new Parser::TreeNode(SEQ, new Parser::TreeNode(STORE, symbolTable.getUniqueSymbol(val)), node);
    if (token.getType() == Token::RPAREN) {
      break;
    }
    check(Token::COMMA, "Expecting COMMA");
    token = lexer.nextToken();
  }
  return node;
}

Parser::TreeNode* Parser::block() {
  check(Token::LBRACE, "Expecting {");
  token = lexer.nextToken();

  // entering a scope!
  symbolTable.enterScope();

  Parser::TreeNode* node = new Parser::TreeNode(SEQ);
  while (token.getType() != Token::RBRACE) {
    Parser::TreeNode* node2 = statement();
    node = new Parser::TreeNode(SEQ, node, node2);
  }
  check(Token::RBRACE, "Expecting }");
  token = lexer.nextToken();

  // exiting the scope!
  symbolTable.exitScope();

  return node;
}

Parser::TreeNode* Parser::function() {
  // entering a scope!
  symbolTable.enterScope();

  check(Token::FUNCTION, "Expecting function");
  token = lexer.nextToken();
  check(Token::IDENT, "Expecting IDENT");
  string val = token.getLexeme();
  token = lexer.nextToken();
  check(Token::LPAREN, "Expecting (");
  token = lexer.nextToken();
  Parser::TreeNode* nodeParams = parameterdefs();
  check(Token::RPAREN, "Expecting )");
  token = lexer.nextToken();
  Parser::TreeNode* nodeLeft = new Parser::TreeNode(SEQ, new Parser::TreeNode(FUNC, val), nodeParams);
  Parser::TreeNode* nodeRight = block();

  // exiting the scope!
  symbolTable.exitScope();
  return new Parser::TreeNode(SEQ, nodeLeft, nodeRight);
}

Parser::TreeNode* Parser::compilationunit() {
  Parser::TreeNode* node = new Parser::TreeNode(SEQ);
  while (token.getType() != Token::ENDOFFILE) {
    Parser::TreeNode* node2 = function();
    node = new Parser::TreeNode(SEQ, node, node2);
  }
  return node;
}
