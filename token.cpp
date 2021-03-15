#include "token.h"

/* constructors */
Token::Token(std::string token_type, std::string token_variable_value)
{
    type = token_type;
    value = token_variable_value;
};

// Token::Token(std::string token_type, char token_char_value)
// {
//     type = token_type;
//     char_value = token_char_value;
// };

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

    // if (this->type == T_IDENTIFIER)
    // {
    //     std::cout << "\t token -> "
    //               << "(" << this->type << ","
    //               << this->string_value << ")" << std::endl;
    // }
    // else
    // {
    //     std::cout << "\t token -> "
    //               << "(" << this->type << ","
    //               << this->char_value << ")" << std::endl;
    // }
};

std::string Token::get_token_type()
{
    return type;
};
std::string Token::get_token_value()
{
    return value;
};