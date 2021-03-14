#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include <vector>
#include <ctype.h>
#include "token.h"

class Lexer
{
private:
    std::vector<Token> tokens_list;
    std::string input_file_name;

public:
    /* constructor */
    Lexer(std::string input_file_name);

    /* will lex the input file and make tokens */
    void run_lexer();

    /* will make a token given a string line */
    void make_tokens(std::string line_to_lex);

    /* will print tokens list */
    void print_tokens_list();

    /* will return a reference to the current tokens_list */
    std::vector<Token> get_tokens_list();
};

#endif