all:
  
	g++ -g mini_python.cpp lexer.h lexer.cpp token.h token.cpp parser.h parser.cpp  -Wall -o  mini_python