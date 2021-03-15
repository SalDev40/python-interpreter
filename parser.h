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

    /* will let us know if its  a binary operator */
    bool is_binary_op;

    Token op;

    /* for single operator */
    TermNode(FactorNode *lf,
             FactorNode *rf,
             bool is_b_op,
             Token opx)
    {
        left_factor = lf;
        right_factor = rf;
        is_binary_op = is_b_op;
        op = opx;
    }

    /* for double operator */
    TermNode(
        TermNode *lbt,
        FactorNode *lf,
        FactorNode *rf,
        bool is_b_op,
        Token opx)
    {
        left_binary_term = lbt;
        left_factor = lf;
        right_factor = rf;
        is_binary_op = is_b_op;
        op = opx;
    }

    TermNode(){};
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

    /* will let us know if its  a binary operator */
    bool is_binary_op;

    Token op;

    /* for single operator */
    ExpressionNode(TermNode *lt,
                   TermNode *rt,
                   bool is_b_op,
                   Token opx)
    {
        left_term = lt;
        right_term = rt;
        is_binary_op = is_b_op;
        op = opx;
    }

    /* for double operator */
    ExpressionNode(
        ExpressionNode *lbe,
        TermNode *lt,
        TermNode *rt,
        bool is_b_op,
        Token opx)
    {
        left_binary_expression = lbe;
        left_term = lt;
        right_term = rt;
        is_binary_op = is_b_op;
        op = opx;
    }

    ExpressionNode(){};
};

// struct ExpressionNode
// {
//     TermNode *left_term;
//     TermNode *right_term;
//     bool is_binary_op;
//     Token op;

//     ExpressionNode(TermNode *lt,
//                    TermNode *rt,
//                    bool is_b_op,
//                    Token opx)
//     {
//         left_term = lt;
//         right_term = rt;
//         is_binary_op = is_b_op;
//         op = opx;
//     }

//     ExpressionNode(){};
// };

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
    // void factor();
    FactorNode *factor();

    /*  */
    // void term();
    TermNode *term();

    /*  */
    ExpressionNode *expression();
    // void expression();

    void print_expression_in_order(ExpressionNode *node);
    void print_term_in_order(TermNode *node, int tabs);

    void run_parser();
};

#endif

// #ifndef PARSER_H
// #define PARSER_H

// #include <iostream>
// #include <string>
// #include <list>
// #include <vector>
// #include <fstream>
// #include <ctype.h>
// #include "token.h"
// #include <map>

// struct NumberNode
// {
//     Token token;
//     NumberNode(Token t) { token = t; }
//     NumberNode() {}
// };

// struct BinaryOpFactors
// {
//     NumberNode *left_node;
//     NumberNode *right_node;
//     std::string op;

//     BinaryOpFactors(NumberNode *left_n,
//                     NumberNode *right_n,
//                     std::string opx)
//     {
//         left_node = left_n;
//         right_node = right_n;
//         op = opx;
//     }

//     BinaryOpFactors(){};
// };
// struct BinaryOpTerms
// {
//     BinaryOpFactors *left_node;
//     BinaryOpFactors *right_node;
//     std::string op;

//     BinaryOpTerms(BinaryOpFactors *left_n,
//                   BinaryOpFactors *right_n,
//                   std::string opx)
//     {
//         left_node = left_n;
//         right_node = right_n;
//         op = opx;
//     }

//     BinaryOpTerms(){};
// };

// class Parser
// {
// private:
//     size_t current_token_index;
//     Token current_token;
//     /* pointer to current tokens list */
//     std::vector<Token> tokens_list;

// public:
//     /* will intialize the parsers token list to the
//        address of the lexer generatedtoken list */
//     Parser();
//     Parser(std::vector<Token> lexer_tokens_list);

//     /* will get the current token from token list */
//     Token get_current_token();

//     /* will get the current token from token list */
//     void advance_current_token_index();

//     /* will check if the current token is a number and return a number node */
//     NumberNode *factor();

//     /*  */
//     BinaryOpFactors *term();

//     /*  */
//     BinaryOpTerms expression();

//     /* will check if the current token is a number and return a number node */
//     void factor1();

//     /*  */
//     void term1();

//     /*  */
//     void expression1();

//     void run_parser();
// };

// #endif
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
    FactorNode *left;
    FactorNode *right;
    bool is_binary_op;
    Token op;

    TermNode(FactorNode *l,
             FactorNode *r,
             bool is_b_op,
             Token opx)
    {
        left = l;
        right = r;
        is_binary_op = is_b_op;
        op = opx;
    }

    TermNode(){};
};

struct ExpressionNode
{
    TermNode *left;
    TermNode *right;
    bool is_binary_op;
    Token op;

    ExpressionNode(TermNode *l,
                   TermNode *r,
                   bool is_b_op,
                   Token opx)
    {
        left = l;
        right = r;
        is_binary_op = is_b_op;
        op = opx;
    }

    ExpressionNode(){};
};

class Parser
{
private:
    size_t current_token_index;
    Token current_token;
    /* pointer to current tokens list */
    std::vector<Token> tokens_list;

    /* pointer to head of AST */
    ExpressionNode *head_ast;

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
    // void factor();
    FactorNode *factor();

    /*  */
    // void term();
    TermNode *term();

    /*  */
    ExpressionNode *expression();
    // void expression();

    void print_expression_post_order(ExpressionNode *node);
    void print_term_post_order(TermNode *node);

    void run_parser();
};

#endif
// #ifndef PARSER_H
// #define PARSER_H

// #include <iostream>
// #include <string>
// #include <list>
// #include <vector>
// #include <fstream>
// #include <ctype.h>
// #include "token.h"
// #include <map>

// struct NumberNode
// {
//     Token token;
//     NumberNode(Token t) { token = t; }
//     NumberNode() {}
// };

// struct BinaryOpFactors
// {
//     NumberNode *left_node;
//     NumberNode *right_node;
//     std::string op;

//     BinaryOpFactors(NumberNode *left_n,
//                     NumberNode *right_n,
//                     std::string opx)
//     {
//         left_node = left_n;
//         right_node = right_n;
//         op = opx;
//     }

//     BinaryOpFactors(){};
// };
// struct BinaryOpTerms
// {
//     BinaryOpFactors *left_node;
//     BinaryOpFactors *right_node;
//     std::string op;

//     BinaryOpTerms(BinaryOpFactors *left_n,
//                   BinaryOpFactors *right_n,
//                   std::string opx)
//     {
//         left_node = left_n;
//         right_node = right_n;
//         op = opx;
//     }

//     BinaryOpTerms(){};
// };

// class Parser
// {
// private:
//     size_t current_token_index;
//     Token current_token;
//     /* pointer to current tokens list */
//     std::vector<Token> tokens_list;

// public:
//     /* will intialize the parsers token list to the
//        address of the lexer generatedtoken list */
//     Parser();
//     Parser(std::vector<Token> lexer_tokens_list);

//     /* will get the current token from token list */
//     Token get_current_token();

//     /* will get the current token from token list */
//     void advance_current_token_index();

//     /* will check if the current token is a number and return a number node */
//     NumberNode *factor();

//     /*  */
//     BinaryOpFactors *term();

//     /*  */
//     BinaryOpTerms expression();

//     /* will check if the current token is a number and return a number node */
//     void factor1();

//     /*  */
//     void term1();

//     /*  */
//     void expression1();

//     void run_parser();
// };

// #endif