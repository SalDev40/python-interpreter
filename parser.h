#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <fstream>
#include <ctype.h>
#include "token.h"
#include <map>

struct NumberNode
{
    Token token;
    NumberNode(Token t) { token = t; }
    NumberNode() {}
};

struct BinaryOpFactors
{
    NumberNode *left_node;
    NumberNode *right_node;
    std::string op;

    BinaryOpFactors(NumberNode *left_n,
                    NumberNode *right_n,
                    std::string opx)
    {
        left_node = left_n;
        right_node = right_n;
        op = opx;
    }

    BinaryOpFactors(){};
};
struct BinaryOpTerms
{
    BinaryOpFactors *left_node;
    BinaryOpFactors *right_node;
    std::string op;

    BinaryOpTerms(BinaryOpFactors *left_n,
                  BinaryOpFactors *right_n,
                  std::string opx)
    {
        left_node = left_n;
        right_node = right_n;
        op = opx;
    }

    BinaryOpTerms(){};
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
    NumberNode *factor();

    /*  */
    BinaryOpFactors *term();

    /*  */
    BinaryOpTerms expression();

    /* will check if the current token is a number and return a number node */
    void factor1();

    /*  */
    void term1();

    /*  */
    void expression1();

    void run_parser();
};

#endif