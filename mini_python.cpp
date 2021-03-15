#include <iostream>
#include "lexer.h"
#include "parser.h"

int main()
{

    std::cout << "LEXER: " << std::endl;
    // Lexer lexer = Lexer("./test.py");
    Lexer lexer = Lexer("./testcases/tc3.py");
    lexer.run_lexer();
    lexer.print_tokens_list();

    std::cout << "*************************************\n";
    std::cout << "*************************************\n";
    std::cout << "*************************************\n";
    std::cout << "PARSER: " << std::endl;
    Parser parser = Parser(lexer.get_tokens_list());

    return 0;
};
