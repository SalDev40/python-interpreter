#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

#define T_END_OF_LIST "EOL"
#define T_STRING "STRING"
#define T_INT "INT"
#define T_DECIMAL "DECIMAL"
#define T_PLUS "PLUS"
#define T_MINUS "MINUS"
#define T_EQUAL "EQUAL"
#define T_MULTIPLY "MULTIPLY"
#define T_DIVIDE "DIVIDE"
#define T_COLON "COLON"
#define T_SEMICOLON "SEMICOLON"
#define T_LEFT_PAREN "LEFT_PAREN"
#define T_RIGHT_PAREN "RIGHT_PAREN"
#define T_LEFT_BRACKET "LEFT_BRACKET"
#define T_RIGHT_BRACKET "RIGHT_BRACKET"
#define T_IDENTIFIER "IDENTIFIER"
#define T_KEYWORD_IF "if"
#define T_KEYWORD_ELIF "elif"
#define T_KEYWORD_ELSE "else"
#define T_KEYWORD_PRINT "print"

class Token
{
private:
    std::string type;
    /* token can either be a string or character */
    std::string value;
    char char_value;

public:
    /* constructors */
    Token(std::string type, std::string value);
    // Token(std::string type, char value);
    Token(std::string type);
    Token();

    /* will print token  */
    void print_token();

    /* getters */
    std::string get_token_type();
    std::string get_token_value();
};

#endif