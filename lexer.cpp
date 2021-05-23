#include "lexer.h"

Lexer::Lexer(std::vector<Token> *tk_list,
             std::unordered_map<std::string, Token> *sym_tab)
{
    tokens_list = tk_list;
    symbol_table = sym_tab;
};

void Lexer::run_lexer(std::string line)
{
    /* make tokens from the given line */
    make_tokens(line);
};

void Lexer::make_tokens(std::string line_to_lex)
{
    for (size_t i = 0; i < line_to_lex.length(); i++)
    {

        if (std::isdigit(line_to_lex[i]))
        {
            std::string str_formed = "";
            /* form an integer */
            while (true)
            {
                str_formed += line_to_lex[i];

                /* if next character isnt a number then break */
                if (!std::isdigit(line_to_lex[i + 1]))
                    break;

                i++;
            }

            tokens_list->push_back(Token(T_INT, str_formed));
        }

        /* check if the current char is a quotation  */
        else if (line_to_lex[i] == '\"')
        {
            std::string str_formed = "";
            i++;
            while (line_to_lex[i] != '\"')
            {
                str_formed += line_to_lex[i];
                i++;
            }
            tokens_list->push_back(Token(T_STRING, str_formed));
        }

        /* check if its  identifier or keyword */

        else if (std::isalpha(line_to_lex[i]))
        {
            std::string str_formed = "";

            /* keep looping until next space is found */
            while (true)
            {
                str_formed += line_to_lex[i];

                /* break if the next character is not part of  the str_formed */
                if (
                    line_to_lex[i + 1] == '=' ||
                    line_to_lex[i + 1] == '\0' ||
                    line_to_lex[i + 1] == '(' ||
                    line_to_lex[i + 1] == ')' ||
                    line_to_lex[i + 1] == '[' ||
                    line_to_lex[i + 1] == ',' ||
                    line_to_lex[i + 1] == ']' ||
                    line_to_lex[i + 1] == '-' ||
                    line_to_lex[i + 1] == '+' ||
                    line_to_lex[i + 1] == '/' ||
                    line_to_lex[i + 1] == '*' ||
                    line_to_lex[i + 1] == '\n' ||
                    line_to_lex[i + 1] == '%' ||
                    line_to_lex[i + 1] == ':' ||
                    line_to_lex[i + 1] == ' ' ||
                    line_to_lex[i + 1] == ';' ||
                    line_to_lex[i + 1] == '\t' ||
                    line_to_lex[i + 1] == '>' ||
                    line_to_lex[i + 1] == '!' ||
                    line_to_lex[i + 1] == '<' ||
                    isspace(line_to_lex[i + 1]) ||
                    i + 1 > line_to_lex.length())
                    break;

                i++;
            };

            /* check if string formed is a key word */
            if (str_formed == T_KEYWORD_LAMBDA)
            {
                tokens_list->push_back(Token(T_KEYWORD_LAMBDA, str_formed));
            }
            else if (str_formed == T_KEYWORD_IF)
            {
                tokens_list->push_back(Token(T_KEYWORD_IF, str_formed));
            }
            else if (str_formed == T_KEYWORD_ELIF)
            {
                tokens_list->push_back(Token(T_KEYWORD_ELIF, str_formed));
            }
            else if (str_formed == T_KEYWORD_ELSE)
            {
                tokens_list->push_back(Token(T_KEYWORD_ELSE, str_formed));
            }
            else if (str_formed == T_KEYWORD_PRINT)
            {
                tokens_list->push_back(Token(T_KEYWORD_PRINT, str_formed));
            }
            else if (str_formed == T_KEYWORD_AND)
            {
                tokens_list->push_back(Token(T_KEYWORD_AND, str_formed));
            }
            else if (str_formed == T_KEYWORD_OR)
            {
                tokens_list->push_back(Token(T_KEYWORD_OR, str_formed));
            }
            else if (str_formed == T_KEYWORD_NOT)
            {
                tokens_list->push_back(Token(T_KEYWORD_NOT, str_formed));
            }
            else
            {
                tokens_list->push_back(Token(T_IDENTIFIER, str_formed));
            }
        }

        /* check if current char is = or == sign */
        else if (line_to_lex[i] == '=')
        {
            if (line_to_lex[i + 1] == '=')
            {
                /* == */
                i++;
                tokens_list->push_back(Token(T_EQUAL_EQUAL,
                                             "=="));
            }
            else
            {
                /* = */
                tokens_list->push_back(Token(T_EQUAL,
                                             std::string(1, '=')));
            }
        }

        /* check if current char is != */
        else if (line_to_lex[i] == '!')
        {
            /* if != */
            if (line_to_lex[i + 1] == '=')
            {
                i++; //advance token an extra time
                tokens_list->push_back(Token(T_NOT_EQUAL,
                                             "!="));
            }
            else
            {
                /* only ! -> error expected != */
            }
        }

        /* check if current char is < or <= */
        else if (line_to_lex[i] == '<')
        {

            if (line_to_lex[i + 1] == '=')
            {
                /* <= */
                i++; //advance token an extra time
                tokens_list->push_back(Token(T_LESS_THAN_EQUAL,
                                             "<="));
            }
            else
            {
                /* < */
                tokens_list->push_back(Token(T_LESS_THAN,
                                             std::string(1, '<')));
            }
        }

        /* check if current char is > or >= */
        else if (line_to_lex[i] == '>')
        {
            if (line_to_lex[i + 1] == '=')
            {
                /* >= */
                i++; //advance token an extra time
                tokens_list->push_back(Token(T_GREATER_THAN_EQUALS,
                                             ">="));
            }
            else
            {
                /* > */
                tokens_list->push_back(Token(T_GREATER_THAN,
                                             std::string(1, '>')));
            }
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '+')
        {
            tokens_list->push_back(Token(T_PLUS,
                                         std::string(1, line_to_lex[i])));
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '-')
        {
            tokens_list->push_back(Token(T_MINUS,
                                         std::string(1, line_to_lex[i])));
        }

        /* check if current char is divide sign */
        else if (line_to_lex[i] == '/')
        {
            tokens_list->push_back(Token(T_DIVIDE,
                                         std::string(1, line_to_lex[i])));
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '*')
        {
            tokens_list->push_back(Token(T_MULTIPLY,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == '[')
        {
            tokens_list->push_back(Token(T_LEFT_BRACKET,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == ']')
        {
            tokens_list->push_back(Token(T_RIGHT_BRACKET,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == '(')
        {
            tokens_list->push_back(Token(T_LEFT_PAREN,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == ')')
        {
            tokens_list->push_back(Token(T_RIGHT_PAREN,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == ':')
        {
            tokens_list->push_back(Token(T_COLON,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == ';')
        {
            tokens_list->push_back(Token(T_SEMICOLON,
                                         std::string(1, line_to_lex[i])));
        }
        else if (line_to_lex[i] == ',')
        {
            tokens_list->push_back(Token(T_COMMA,
                                         std::string(1, line_to_lex[i])));
        }

        else
        {
        }
    }
};

void Lexer::print_tokens_list()
{

    for (auto it = tokens_list->begin(); it != tokens_list->end(); it++)
    {
        it->print_token();
    }
};
