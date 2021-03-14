#include "token.h"

/* constructors */
Token::Token(std::string token_type, std::string token_variable_value)
{
    type = token_type;
    string_value = token_variable_value;
};

Token::Token(std::string token_type, char token_char_value)
{
    type = token_type;
    char_value = token_char_value;
};

Token::Token(std::string token_type)
{
    type = token_type;
};

void Token::print_token()
{
    if (this->type == T_IDENTIFIER)
    {
        std::cout << "\t token -> "
                  << " ( " << this->type << " , "
                  << this->string_value << " ) " << std::endl;
    }
    else
    {
        std::cout << "\t token -> "
                  << " ( " << this->type << " , "
                  << this->char_value << " ) " << std::endl;
    }
};







std::string Token::get_token_type()
{
    return type;
};
std::string Token::get_token_string_value()
{
    return string_value;
};
char Token::get_token_char_value()
{
    return char_value;
};