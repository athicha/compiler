OPTS = -g -c -Wall -Werror -Wno-non-template-friend

microc: token.o lexer.o parser.o lexer.o 
	g++ -o microc microc.cc token.o lexer.o parser.o symboltable.o stack.o

parser.o: parser.h parser.cc lexer.o symboltable.o
	g++ $(OPTS) parser.cc

symboltable.o: symboltable.h symboltable.cc stack.o
	g++ $(OPTS) symboltable.cc

token.o: token.h token.cc
	g++ $(OPTS) token.cc

lexer.o : lexer.h lexer.cc token.o
	g++ $(OPTS) lexer.cc

stack.o: stack.cc stack.h
	g++ $(OPTS) stack.cc

clean:
	rm -f microc *.o
