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

struct FactorNode
{
    Token token;
    FactorNode(Token t) { token = t; }
    FactorNode() {}
};

struct TermNode
{
    /* if only a left factor */
    FactorNode *left_factor;

    /* if both a left and right factor */
    FactorNode *right_factor;

    /* 
    used for nesting, either a term will be a factor node
    or a binary node indicating two number operation
    */
    TermNode *left_binary_term;

    Token op;

    /* for single operator */
    TermNode(FactorNode *lf,
             FactorNode *rf,
             Token opx)
    {
        left_factor = lf;
        right_factor = rf;
        op = opx;
    }

    /* for double operator */
    TermNode(
        TermNode *lbt,
        FactorNode *lf,
        FactorNode *rf,
        Token opx)
    {
        left_binary_term = lbt;
        left_factor = lf;
        right_factor = rf;
        op = opx;
    }

    TermNode()
    {
        // left_binary_term = new TermNode();
        left_factor = nullptr;
        right_factor = nullptr;
        op = Token();
    };
};

struct ExpressionNode
{
    /* if only a left term */
    TermNode *left_term;

    /* 
    used for nesting, either a expression will be a term node
    or a binary node indicating multi term operation
    */

    /* if both a left and right term */
    TermNode *right_term;
    ExpressionNode *left_binary_expression;

    Token op;

    /* for single operator */
    ExpressionNode(TermNode *lt,
                   TermNode *rt,
                   Token opx)
    {
        left_term = lt;
        right_term = rt;
        op = opx;
    }

    /* for double operator */
    ExpressionNode(
        ExpressionNode *lbe,
        TermNode *lt,
        TermNode *rt,
        Token opx)
    {
        left_binary_expression = lbe;
        left_term = lt;
        right_term = rt;
        op = opx;
    }

    ExpressionNode()
    {
        // left_binary_expression = new ExpressionNode();
        left_term = nullptr;
        right_term = nullptr;
        op = Token();
    };
};

class Parser
{
private:
    size_t current_token_index;
    Token current_token;
    /* pointer to current tokens list */
    std::vector<Token> tokens_list;

    /* pointer to head of AST */
    ExpressionNode *head_ast = nullptr;

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
    FactorNode *factor();

    /*  */
    TermNode *term();

    /*  */
    ExpressionNode *expression();

    void interpret_expression_in_order(ExpressionNode *node);
    void interpret_term_in_order(TermNode *node, int tabs);

    void print_expression_in_order(ExpressionNode *node);
    void print_term_in_order(TermNode *node, int tabs);

    void print_expression_pre_order(ExpressionNode *node);
    void print_term_pre_order(TermNode *node, int tabs);

    void run_parser();
};

#endif