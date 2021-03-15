#include <iostream>
#include "lexer.h"
#include "parser.h"

int main()
{

    std::cout << "*************************************\n";
    std::cout << "*************************************\n";
    std::cout << "LEXER: " << std::endl;
    std::cout << "*************************************\n";
    std::cout << "*************************************\n";

    // Lexer lexer = Lexer("./test.py");
    // Lexer lexer = Lexer("./testcases/tc3.py");
    Lexer lexer = Lexer("./testcases/tc0.py");
    lexer.run_lexer();
    lexer.print_tokens_list();

    std::cout << "*************************************\n";
    std::cout << "*************************************\n";
    std::cout << "PARSER: " << std::endl;
    std::cout << "*************************************\n";
    std::cout << "*************************************\n";

    Parser parser = Parser(lexer.get_tokens_list());
    parser.run_parser();
    return 0;
};
