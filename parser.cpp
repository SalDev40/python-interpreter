#include "parser.h"
Parser::Parser(){};

Parser::Parser(std::vector<Token> lexer_tokens_list)
{
    current_token_index = -1;
    tokens_list = lexer_tokens_list;

    /* get a token */
    advance_current_token_index();
}

void Parser::advance_current_token_index()
{
    current_token_index++;
    if (current_token_index < tokens_list.size())
        /* get the current token */
        current_token = tokens_list[current_token_index];
}

NumberNode *Parser::factor()
{
    /* if the current token is an integer or decimal make a number node for AST */
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        return new NumberNode(current_token);
    }
}

BinaryOpFactors *Parser::term()
{
    /* get left factor */

    /* @TODO: issue is left_node can be either a numbernode or binaryOpnode */
    NumberNode *left_node = factor();

    std::cout << "left factor -> "
              << left_node->token.get_token_type() << std::endl;

    /* advance to next token */
    // advance_current_token_index();

    std::cout << "current_token -> "
              << current_token.get_token_type() << std::endl;

    BinaryOpFactors *binary_operation = new BinaryOpFactors();

    /* if the token is multiply or divide */
    while (current_token.get_token_type() == T_MULTIPLY ||
           current_token.get_token_type() == T_DIVIDE)
    {

        /* get the token */
        Token operator_token = current_token;
        std::cout << "operator -> "
                  << operator_token.get_token_type() << std::endl;

        /* advance to next token */
        advance_current_token_index();

        /* get right factor */
        NumberNode *right_node = factor();
        std::cout << "right factor -> "
                  << right_node->token.get_token_type() << std::endl;

        /* do a binary operation between both factors */
        binary_operation =
            new BinaryOpFactors(
                left_node,
                right_node,
                operator_token.get_token_value());
    }

    // std::cout << " " << binary_operation->left_node->token.get_token_type();
    // std::cout << " " << binary_operation->left_node->token.get_token_value();

    // std::cout << " " << binary_operation->op;

    // std::cout << " " << binary_operation->right_node->token.get_token_type();
    // std::cout << " " << binary_operation->right_node->token.get_token_value();

    return binary_operation;
}

BinaryOpTerms Parser::expression()
{
    /* get left term */
    BinaryOpFactors *left_node = term();

    BinaryOpTerms binary_operation = BinaryOpTerms();

    /* if the token is multiply or divide */
    while (
        current_token.get_token_type() == T_PLUS ||
        current_token.get_token_type() == T_MINUS)
    {

        /* get the token */
        Token operator_token = current_token;

        /* advance to next token */
        advance_current_token_index();

        /* get right term */
        BinaryOpFactors *right_node = term();

        /* do a binary operation between both terms */
        binary_operation =
            BinaryOpTerms(left_node,
                          right_node,
                          operator_token.get_token_value());
    }

    return binary_operation;
}

void Parser::factor1()
{
    std::cout << "\t\t\tEnter <factor>\n"
              << std::endl;

    /* if the current token is an integer or decimal make a number node for AST */
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        std::cout << "\t\t\t";
        current_token.print_token();
    }

    /* advance token */
    advance_current_token_index();

    std::cout << "\n\t\t\tExit <factor>\n"
              << std::endl;
}

void Parser::term1()
{
    std::cout << "\t\tEnter <term>\n"
              << std::endl;
    /* get left factor */
    factor1();

    /* if the token is multiply or divide */
    while (current_token.get_token_type() == T_MULTIPLY ||
           current_token.get_token_type() == T_DIVIDE)
    {

        std::cout << "\t\t --------> ";
        current_token.print_token();

        /* advance to next token */
        advance_current_token_index();

        /* get right factor */
        factor1();
    }

    // advance_current_token_index();

    std::cout << "\n\t\tExit <term>\n"
              << std::endl;
}

void Parser::expression1()
{
    std::cout << "\tEnter <expr>\n"
              << std::endl;

    /* get left term */
    term1();

    /* if the token is plus or minus */
    while (
        current_token.get_token_type() == T_PLUS ||
        current_token.get_token_type() == T_MINUS)
    {

        std::cout << "\t -------> ";
        current_token.print_token();
        /* advance to next token */
        advance_current_token_index();

        /* get right term */
        term1();
    }
    std::cout << "\n\tExit <expr>\n"
              << std::endl;
}

void Parser::run_parser()
{
    // auto AST = this->expression();
    this->expression1();
}