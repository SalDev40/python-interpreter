#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <ctype.h>
#include "token.h"

struct NumberNode
{
    Token number_token;
    NumberNode(Token token) { number_token = token; }
};

struct BinaryOp
{
    NumberNode left_node;
    NumberNode right_node;
    char op;
};

class Parser
{
private:
    size_t current_token_index;
    Token current_token;
    /* pointer to current tokens list */
    std::vector<Token> tokens_list;

public:
    /* will intialize the parsers token list to the 
       address of the lexer generatedtoken list */
    Parser();
    Parser(std::vector<Token> lexer_tokens_list);

    /* will get the current token from token list */
    Token get_current_token();

    /* will get the current token from token list */
    void advance_current_token_index();

    /* will check if the current token is a number and return a number node */
    NumberNode get_number_factor();
};

#endif