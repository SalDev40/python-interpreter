#include "lexer.h"

Lexer::Lexer(std::string file_name)
{
    input_file_name = file_name;
};

void Lexer::run_lexer()
{
    std::cout << "reading in file " << input_file_name << std::endl;

    /* read input file line by line */
    std::ifstream input(input_file_name);
    for (std::string line; getline(input, line);)
    {
        std::cout << "\n***************************** " << std::endl;
        std::cout << "current line to lex -> " << line << std::endl;
        std::cout << "***************************** " << std::endl;

        /* make tokens from the given line */
        make_tokens(line);
    }

    input.close();
};

void Lexer::make_tokens(std::string line_to_lex)
{
    for (size_t i = 0; i < line_to_lex.length(); i++)
    {
        std::cout << "current symbol:  " << line_to_lex[i];
        /* skip if its a space */
        if (std::isspace((line_to_lex[i])))
        {
            std::cout << " -> skipping space" << std::endl;
        }

        /* @TODO: decimal vs int check if current char is a number */
        else if (std::isdigit(line_to_lex[i]))
        {
            tokens_list.push_back(Token(T_INT, line_to_lex[i]));

            /* @TODO: throw error if after number is a char -> invalid variable name */

            /* @TODO:  check if its a decimal */
            std::cout
                << " -> is a digit";
        }

        /* @TODO: check if current char is a variable */
        else if (std::isalpha(line_to_lex[i]))
        {
            std::cout << "\n -> checking if its  a variable" << std::endl;
            std::string str_formed = " ";

            /* keep looping until next space is found */
            while (!std::isspace(line_to_lex[i]))
            {
                std::cout << "\n\tnext symbol:  " << line_to_lex[i];
                str_formed += line_to_lex[i];

                /* break if the next character is not part of a the str_formed */
                if (
                    line_to_lex[i + 1] == '=' ||
                    line_to_lex[i + 1] == '(' ||
                    line_to_lex[i + 1] == ')' ||
                    line_to_lex[i + 1] == '-' ||
                    line_to_lex[i + 1] == '+' ||
                    line_to_lex[i + 1] == '\\' ||
                    line_to_lex[i + 1] == '*' ||
                    line_to_lex[i + 1] == '\n' ||
                    line_to_lex[i + 1] == '%' ||
                    i + 1 > line_to_lex.length())
                    break;

                i++;
            };
            std::cout << std::endl;

            /* @TODO: check if string formed is a keyword */
            if (str_formed == T_KEYWORD_IF)
            {
                std::cout << "\n -> is a keyword -> " << str_formed << std::endl;
                tokens_list.push_back(Token(T_KEYWORD_IF, str_formed));
            }
            if (str_formed == T_KEYWORD_ELIF)
            {
                std::cout << "\n -> is a keyword -> " << str_formed << std::endl;
                tokens_list.push_back(Token(T_KEYWORD_ELIF, str_formed));
            }
            else if (str_formed == T_KEYWORD_ELSE)
            {
                std::cout << "\n -> is a keyword -> " << str_formed << std::endl;
                tokens_list.push_back(Token(T_KEYWORD_ELSE, str_formed));
            }
            else if (str_formed == T_KEYWORD_PRINT)
            {
                std::cout << "\n -> is a keyword -> " << str_formed << std::endl;
                tokens_list.push_back(Token(T_KEYWORD_PRINT, str_formed));
            }
            else
            {
                /* @TODO: if string formed is a variable store in symbol map  */
                std::cout << "\n -> is a variable -> " << str_formed << std::endl;
                tokens_list.push_back(Token(T_IDENTIFIER, str_formed));
            }
        }

        /* check if current char is equal sign */
        else if (line_to_lex[i] == '=')
        {
            tokens_list.push_back(Token(T_EQUAL, line_to_lex[i]));
            std::cout << " -> is equal sign";
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '+')
        {
            tokens_list.push_back(Token(T_PLUS, line_to_lex[i]));
            std::cout << " -> is plus sign";
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '-')
        {
            tokens_list.push_back(Token(T_MINUS, line_to_lex[i]));
            std::cout << " -> is minus sign";
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '\\')
        {
            tokens_list.push_back(Token(T_DIVIDE, line_to_lex[i]));
            std::cout << " -> is divide sign";
        }

        /* check if current char is plus sign */
        else if (line_to_lex[i] == '*')
        {
            tokens_list.push_back(Token(T_MULTIPLY, line_to_lex[i]));
            std::cout << " -> is multiply sign";
        }

        std::cout << std::endl;
    }
};

void Lexer::print_tokens_list()
{
    std::cout << "\n***************************** " << std::endl;
    std::cout << "-> current tokens list: " << std::endl;
    std::cout << "***************************** " << std::endl;

    for (auto i : tokens_list)
    {
        i.print_token();
    }
};

std::vector<Token> Lexer::get_tokens_list()
{
    return tokens_list;
};