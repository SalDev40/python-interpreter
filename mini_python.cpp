#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv)
{

    std::unordered_map<std::string, Token> symbol_table;
    std::string input_file_name = argv[1];

    std::ifstream input(input_file_name);

    bool should_do_if_statement = true;
    bool should_do_else_statement = true;
    bool is_if_done = true;

    for (std::string line; getline(input, line);)
    {
        /* skip all empty and commented out lines */
        if (line == "" || line[0] == '#')
        {
            continue;
        }

        if (line[line.length() - 1] == '\0')
        {
            line.pop_back();
        }
        if (line[line.length() - 1] == ';')
        {
            line.pop_back();
        }

        if (should_do_if_statement == false && is_if_done == true)
        {
            if (line[0] == ' ')
                continue;
        }

        else if (should_do_else_statement == false &&
                 is_if_done == true)
        {
            if (line[0] == ' ')
                continue;
        }

        std::vector<Token> tokens_list;

        Lexer lexer = Lexer(
            &tokens_list,
            &symbol_table);

        lexer.run_lexer(line);
        Parser parser = Parser(&tokens_list,
                               &symbol_table,
                               &should_do_if_statement,
                               &should_do_else_statement,
                               &is_if_done

        );
        parser.run_parser(line);


        should_do_if_statement = true;

       
    }
    input.close();

    return 0;
};
