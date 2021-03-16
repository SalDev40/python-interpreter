#include "token.h"

/* constructors */
Token::Token(std::string token_type, std::string token_variable_value)
{
    type = token_type;
    value = token_variable_value;
};

Token::Token(std::string token_type)
{
    type = token_type;
};
Token::Token()
{
    type = "N/A";
};

void Token::print_token()
{

    std::cout << "\t token -> "
              << "(" << this->type << ","
              << this->value << ")\n"
              << std::endl;
};

std::string Token::get_token_type()
{
    return type;
};
std::string Token::get_token_value()
{
    return value;
};