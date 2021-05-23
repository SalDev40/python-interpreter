#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>

#define T_END_OF_LIST "EOL"

#define T_STRING "STRING"
#define T_INT "INT"
#define T_DECIMAL "DECIMAL"
#define T_LIST "LIST"

#define T_PLUS "PLUS"
#define T_MINUS "MINUS"
#define T_EQUAL "EQUAL"
#define T_MULTIPLY "MULTIPLY"
#define T_DIVIDE "DIVIDE"

#define T_COLON "COLON"
#define T_COMMA "COMMA"
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
#define T_KEYWORD_LAMBDA "lambda"


#define T_KEYWORD_AND "AND"
#define T_KEYWORD_NOT "NOT"
#define T_KEYWORD_OR "OR"

#define T_NOT_EQUAL "NOT_EQUAL"
#define T_EQUAL_EQUAL "EQUAL_EQUAL"
#define T_LESS_THAN "LESS_THAN"
#define T_LESS_THAN_EQUAL "LESS_THAN_OR_EQUAL"
#define T_GREATER_THAN "GREATER_THAN"
#define T_GREATER_THAN_EQUALS "GREATER_THAN_OR_EQUAL"





class Token
{
private:
    std::string type;
    std::string value;

public:
    /* constructors */
    Token(std::string type, std::string value);
    Token(std::string type);
    Token();

    /* will print token  */
    void print_token();

    /* getters */
    std::string get_token_type();
    std::string get_token_value();
};

#endif