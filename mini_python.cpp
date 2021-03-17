#include <iostream>
#include <fstream>
#include "lexer.h"
#include "parser.h"

void print_symbol_table(std::unordered_map<std::string, Token> *symbol_table)
{
    std::cout << "\n*************************************\n";
    std::cout << "SYMBOL TABLE: " << std::endl;
    std::cout << "*************************************\n";

    for (auto it = symbol_table->begin(); it != symbol_table->end(); it++)
    {
        std::cout << "variable_name: " << it->first;
        it->second.print_token();
    }
}

int main()
{

    std::unordered_map<std::string, Token> symbol_table;
    std::string input_file_name = "./testcases/tc1.py";

    std::ifstream input(input_file_name);

    std::cout << "reading in file "
              << input_file_name << std::endl;

    std::string input_dummy;

    /* @TODO: error intepreting last line for some reason */
    for (std::string line; getline(input, line);)
    {

        if (line == "" || line[0] == '#')
        {
            continue;
        }

        if (line[line.length() - 1] == '\0')
        {
            std::cout << "trimming line " << std::endl;
            line.pop_back();
        }

        std::cout << "\n\n*************************************\n";
        std::cout << "*************************************\n";
        std::cout << "LEXER: " << std::endl;
        std::cout << "*************************************\n";
        std::cout << "*************************************\n";

        std::vector<Token> tokens_list;

        Lexer lexer = Lexer(
            &tokens_list,
            &symbol_table);

        lexer.run_lexer(line);
        lexer.print_tokens_list();

        std::cout << "*************************************\n";
        std::cout << "*************************************\n";
        std::cout << "PARSER: " << std::endl;
        std::cout << "*************************************\n";
        std::cout << "*************************************\n";

        Parser parser = Parser(&tokens_list,
                               &symbol_table);
        parser.run_parser();

        print_symbol_table(&symbol_table);

        // std::cin >> input_dummy;
    }
    input.close();

    return 0;
};
