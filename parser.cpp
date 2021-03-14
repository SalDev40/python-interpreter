#include "parser.h"
Parser::Parser(){};

Parser::Parser(std::vector<Token> lexer_tokens_list)
{
    current_token_index = -1;
    tokens_list = lexer_tokens_list;
}

void Parser::advance_current_token_index()
{
    current_token_index++;
    if (current_token_index < tokens_list.size())
        /* get the current token */
        current_token = tokens_list[current_token_index];
}

NumberNode Parser::get_number_factor()
{
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        return NumberNode(current_token);
    }
}