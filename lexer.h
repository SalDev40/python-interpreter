#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <ctype.h>
#include <map>
#include "token.h"
#include "parser.h"

class Lexer
{
private:
    std::vector<Token>* tokens_list;
    std::unordered_map<std::string, Token>* symbol_table;

public:
     /* constructor */
    Lexer(std::vector<Token> *tk_list,
          std::unordered_map<std::string, Token> *sym_tab);

    /* will lex the input file and make tokens */
    void run_lexer(std::string line);

    /* will make a token given a string line */
    void make_tokens(std::string line_to_lex);

    /* will print tokens list */
    void print_tokens_list();

    /* will return a reference to the current tokens_list */
    std::vector<Token> get_tokens_list();

    /* will return a reference to the symbol_table */
    std::unordered_map<std::string, Token> *get_symbol_table();

    void print_symbol_table();
};

#endif