#include "parser.h"
void Parser::print_symbol_table(std::unordered_map<std::string, Token> *symbol_table)
{
    std::cout << "\n*************************************\n";
    std::cout << "SYMBOL TABLE: " << std::endl;
    std::cout << "*************************************\n";

    for (auto it = symbol_table->begin(); it != symbol_table->end(); it++)
    {
        std::cout << "variable_name: " << it->first;
        it->second.print_token();
    }

    std::cout << "\n*************************************\n";
    std::cout << "*************************************\n";
}

Parser::Parser(){};

Parser::Parser(std::vector<Token> *lexer_tokens_list,
               std::unordered_map<std::string, Token> *lexer_symbol_table)

{
    current_token_index = -1;
    tokens_list = lexer_tokens_list;
    symbol_table = lexer_symbol_table;

    /* get a token */
    advance_current_token_index();
}

Token Parser::search_key_in_map(std::string variable_name)
{

    std::unordered_map<std::string, Token> new_sym_tab;
    for (auto it = symbol_table->begin(); it != symbol_table->end(); it++)
    {
        new_sym_tab.insert(std::make_pair(
            it->first,
            it->second));
    }

    for (auto it = symbol_table->begin(); it != symbol_table->end(); it++)
    {
        for (auto i : it->first)
        {
        }
        std::cout << "sizeof: " << sizeof(it->first) << " : "
                  << sizeof(variable_name) << std::endl;

        std::cout << "length: " << it->first.length() << " : "
                  << variable_name.length() << std::endl;

        std::cout << "value: " << it->first << " == " << variable_name
                  << " ? " << (it->first == variable_name) << std::endl;

        if (it->first == variable_name)
        {
            return it->second;
        }
        std::cout << std::endl;
    }

    if (new_sym_tab.find(variable_name) == new_sym_tab.end())
    {
        std::cout << "not found" << std::endl;
    }
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::advance_current_token_index()
{
    current_token_index++;
    if (current_token_index < tokens_list->size())
        /* get the current token */
        current_token = (*tokens_list)[current_token_index];
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

FactorNode *Parser::factor()
{
    std::cout << "\t\t\tEnter <factor>\n"
              << std::endl;

    FactorNode *factor_node = new FactorNode();
    factor_node->token = Token();

    /* if the current token is an plus or minus */

    /* @TODO: unary operator */
    // if (current_token.get_token_type() == T_PLUS ||
    //     current_token.get_token_type() == T_MINUS)
    // {
    //     std::cout << "\t\t\t";
    //     current_token.print_token();

    //     FactorNode* factor_node_again = factor();
    //     factor_node->token = factor_node_again->token;
    //     /* advance token */
    //     advance_current_token_index();
    // }

    /* if  its a variable, get its value */
    if (current_token.get_token_type() == T_IDENTIFIER)
    {
        std::cout << "\t\t\t\t IDENTIFIER";

        current_token.print_token();

        // Token variable_data = search_key_in_map(current_token.get_token_value());

        auto variable_data = symbol_table->find(current_token.get_token_value());

        if (variable_data == symbol_table->end())
        {
            std::cout << "Variable not defined" << std::endl;
        }

        /* set the factor value to the variable tokens value */
        factor_node->token =
            Token(variable_data->second.get_token_type(),
                  variable_data->second.get_token_value());

        factor_node->token.print_token();
        // print_symbol_table(symbol_table);

        /* advance token */
        advance_current_token_index();
    }

    /* if the current token is an integer or decimal make a number node for AST */
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        std::cout << "\t\t\t\t INT/DECIMAL";
        current_token.print_token();
        factor_node->token = current_token;

        /* advance token */
        advance_current_token_index();
    }

    /* if the current token is a LEFT_PAREN or RIGHT_PARENT */
    if (current_token.get_token_type() == T_LEFT_PAREN)
    {
        std::cout << "\t\t\t\t LEFT_PAREN";
        current_token.print_token();

        /* advance token */
        advance_current_token_index();

        /* solve inner expression and save result value */
        ExpressionNode *expr = expression();
        interpret_expression_in_order(expr, 1, (expr->right_term != nullptr));

        std::cout << "nested expression: ";
        expr->left_term->left_factor->token.print_token();

        /* if we get right paren, inner expression is done */
        if (current_token.get_token_type() == T_RIGHT_PAREN)
        {
            std::cout << "\t\t\t\t\t RIGHT_PAREN";
            current_token.print_token();

            /* advance past the inner expression right paren */
            advance_current_token_index();

            /* return solved inner expression token  */
            factor_node->token = expr->left_term->left_factor->token;
        }
    }

    std::cout << "\n\t\t\tExit <factor>\n"
              << std::endl;

    return factor_node;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

TermNode *Parser::term()
{

    std::cout << "\t\tEnter <term>\n"
              << std::endl;

    /* root term node will be returned to expression  */
    TermNode *term_node = new TermNode();

    /* will  be a tree itself, holding value of nested factors */
    term_node->left_binary_term = new TermNode();

    /* get left factor */
    term_node->left_factor = factor();
    term_node->left_binary_term->left_factor = term_node->left_factor;

    /* if the token is multiply or divide */
    while (current_token.get_token_type() == T_MULTIPLY ||
           current_token.get_token_type() == T_DIVIDE)
    {
        std::cout << "\t\t --------> ";
        current_token.print_token();

        term_node->op = current_token;

        /* advance to next token */
        advance_current_token_index();

        /* get right factor */
        term_node->right_factor = factor();

        /* re assign the left node of term node to be a binary factor node */
        term_node->left_binary_term = new TermNode(
            term_node->left_binary_term, //make this new node leftbinterm previous leftbinterm
            term_node->left_binary_term->left_factor,
            term_node->right_factor,
            term_node->op

        );
    }

    /* drop first node since it duplicates */
    term_node->left_binary_term = term_node->left_binary_term->left_binary_term;

    std::cout << "\n\t\tExit <term>\n"
              << std::endl;

    return term_node;
}
/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

ExpressionNode *Parser::expression()
{
    std::cout << "\tEnter <expr>\n"
              << std::endl;
    /* root expression node aka head of the tree */
    ExpressionNode *expression_node = new ExpressionNode();

    /* will hold nested expressions that are trees  */
    expression_node->left_binary_expression = new ExpressionNode();

    /* get left term */
    expression_node->left_term = term();
    expression_node->left_binary_expression->left_term = expression_node->left_term;

    /* if the token is plus or minus */
    while (
        current_token.get_token_type() == T_PLUS ||
        current_token.get_token_type() == T_MINUS)
    {
        std::cout << "\t -------> ";
        current_token.print_token();

        expression_node->op = current_token;

        /* advance to next token */
        advance_current_token_index();

        /* get right term */
        expression_node->right_term = term();

        /* re assign the left node of expression node to be a binary term node */
        expression_node->left_binary_expression = new ExpressionNode(
            expression_node->left_binary_expression,
            expression_node->left_binary_expression->left_term,
            expression_node->right_term,
            expression_node->op

        );
    }

    /* drop first node since it duplicates */
    expression_node->left_binary_expression =
        expression_node->left_binary_expression->left_binary_expression;

    std::cout << "\n\tExit <expr>\n"
              << std::endl;

    return expression_node;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::interpret_expression_in_order(ExpressionNode *node,
                                           int tabs,
                                           bool has_a_right_initial_tree)
{

    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }
    std::cout << "<expr> " << tabs << " ENTER INTERPRET INORDER <expr>\n"
              << std::endl;

    /* to store results from terms, will be used in previous recursive expressions */
    std::string right_result_token;
    std::string left_result_token;
    std::string right_result;
    std::string left_result;

    // if (node == nullptr)
    /* if it has a right tree recurse all the way down to second to last term */
    if (has_a_right_initial_tree && node->right_term == nullptr)
    {
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        std::cout << "End of Tree";
        std::cout << std::endl
                  << std::endl;
        return;
    }

    /* if it has no right tree, loop all the way recursively to the bottom */
    else if (!has_a_right_initial_tree && node == nullptr)
    {
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        std::cout << "End of Tree";
        std::cout << std::endl
                  << std::endl;
        return;
    }
    else
    {
        /* traverse all expressions till you get to last expression of terms */
        interpret_expression_in_order(node->left_binary_expression,
                                      tabs + 1,
                                      has_a_right_initial_tree);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }

        /* traverse left side of the expression node */
        if (node->left_term)
        {
            std::cout << "\t<expr> ****** ENTER INORDER LEFT TERM ****** <expr>\n\n";

            /* traverse the left term tree */
            interpret_term_in_order(node->left_term, tabs + 3);

            /* get the final left result value from the top node */
            left_result =
                node->left_term->left_factor->token.get_token_value();

            left_result_token =
                node->left_term->left_factor->token.get_token_type();

            /* print nested tabs */
            for (int i = 0; i < tabs + 2; i++)
            {
                std::cout << "\t";
            }

            std::cout << "\t!!!!! LEFT RESULT = "
                      << left_result << std::endl
                      << std::endl;

            /* print nested tabs */
            for (int i = 0; i < tabs + 2; i++)
            {
                std::cout << "\t";
            }

            std::cout << "\t<expr> ****** EXIT INORDER LEFT TERM ****** <expr>\n\n";
        }

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        /* print operator to use */
        std::cout << " OP -> \t\t\t "
                  << node->op.get_token_value() << "\n\n"
                  << std::endl;

        /* traverse right side of the expression node */
        if (node->right_term)
        {
            /* print nested tabs */
            for (int i = 0; i < tabs + 2; i++)
            {
                std::cout << "\t";
            }
            std::cout << "\t<expr> ****** ENTER INORDER RIGHT TERM ******<expr>\n\n";

            /* traverse the right term tree */
            interpret_term_in_order(node->right_term, tabs + 3);

            right_result = node->right_term->left_factor->token.get_token_value();
            right_result_token = node->right_term->left_factor->token.get_token_type();

            /* print nested tabs */
            for (int i = 0; i < tabs + 2; i++)
            {
                std::cout << "\t";
            }

            std::cout << "\t!!!!!! RIGHT RESULT = "
                      << right_result << std::endl
                      << std::endl;

            /* print nested tabs */
            for (int i = 0; i < tabs + 2; i++)
            {
                std::cout << "\t";
            }

            std::cout << "\t<expr> ****** EXIT INORDER RIGHT TERM ******<expr>\n\n";
        }

        /* calculate the final result and store it in left node of the current node
           next expression before it will read from it recursively
        */

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        if (node->left_term && !node->right_term)
        {
            std::cout << "!!!!!! LEFT ONLY TOTAL = " << stoi(left_result) << std::endl
                      << std::endl;

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();
            node->left_term->left_factor->token =
                Token(left_result_token,
                      std::to_string(stoi(left_result)));

            node->left_term->left_factor->token.print_token();
        }

        if (node->right_term && !node->left_term)
        {
            std::cout << "!!!!!! RIGHT ONLY TOTAL = " << stoi(right_result) << std::endl
                      << std::endl;

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();
            node->left_term->left_factor->token =
                Token(right_result_token,
                      std::to_string(stoi(right_result)));

            node->left_term->left_factor->token.print_token();
        }

        /* add left and right terms */
        if (node->left_term && node->right_term)
        {
            if (node->op.get_token_type() == T_PLUS)
            {
                std::cout << "!!!!!! L + R TOTAL = "
                          << stoi(left_result) + stoi(right_result) << std::endl
                          << std::endl;

                /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
                delete node->left_term;
                node->left_term = new TermNode();
                node->left_term->left_factor = new FactorNode();

                /* @TODO: check to see if one is a decimal */

                node->left_term->left_factor->token =
                    Token(left_result_token,
                          std::to_string(stoi(left_result) + stoi(right_result)));

                node->left_term->left_factor->token.print_token();
            }

            if (node->op.get_token_type() == T_MINUS)
            {
                std::cout << "!!!!!! L - R  TOTAL = "
                          << stoi(left_result) - stoi(right_result) << std::endl
                          << std::endl;

                /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
                delete node->left_term;
                node->left_term = new TermNode();
                node->left_term->left_factor = new FactorNode();

                node->left_term->left_factor->token =
                    Token(left_result_token,
                          std::to_string(stoi(left_result) - stoi(right_result)));

                node->left_term->left_factor->token.print_token();
            }
        }
    }

    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }
    std::cout << "<expr>  " << tabs << " EXIT INORDER <expr>\n"
              << std::endl;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::interpret_term_in_order(TermNode *node, int tabs)
{
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }

    std::cout << "<term> "
              << tabs << " ENTER INORDER  <term>\n"
              << std::endl;

    /* issue is that sometimes the left->binary even tho its empty
    its not null its 0xf , but the last node right_factor is always null
    */
    if (node->right_factor == nullptr)
    {
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        std::cout << "End of Tree";
        std::cout << std::endl
                  << std::endl;
        return;
    }
    else
    {
        /* traverse all the way down to the bottom node */
        interpret_term_in_order(node->left_binary_term, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }

        if (node->left_factor)
            std::cout << "(" << node->left_factor->token.get_token_type()
                      << "," << node->left_factor->token.get_token_value()
                      << ")";

        std::cout << " " << node->op.get_token_value() << " ";

        if (node->right_factor)
            std::cout << "(" << node->right_factor->token.get_token_type()
                      << "," << node->right_factor->token.get_token_value()
                      << ")";

        /* perform mathematical operation on node */
        if (node->left_factor && node->right_factor)
        {
            /* calculate result of both operands */
            if (node->left_factor->token.get_token_type() == T_INT &&
                node->right_factor->token.get_token_type() == T_INT)
            {

                if (node->op.get_token_type() == T_MULTIPLY)
                {
                    auto result =
                        std::stoi(node->left_factor->token.get_token_value()) *
                        std::stoi(node->right_factor->token.get_token_value());

                    /* store result in left factor of current node
                       next node will  read from this token and use it
                    */
                    node->left_factor->token =
                        Token(node->left_factor->token.get_token_type(), std::to_string(result));

                    node->left_factor->token.print_token();

                    std::cout << " = " << result;
                }

                if (node->op.get_token_type() == T_DIVIDE)
                {
                    auto result =
                        std::stoi(node->left_factor->token.get_token_value()) /
                        std::stoi(node->right_factor->token.get_token_value());

                    /* store result in left factor of current node
                       next node will  read from this token and use it
                    */
                    node->left_factor->token =
                        Token(node->left_factor->token.get_token_type(),
                              std::to_string(result));

                    node->left_factor->token.print_token();

                    std::cout << " = " << result;
                }
                // if (node->op.get_token_type() == T_PLUS)
                // {
                //     auto result =
                //         std::stoi(node->left_factor->token.get_token_value()) +
                //         std::stoi(node->right_factor->token.get_token_value());

                //     /* store result in left factor of current node
                //        next node will  read from this token and use it
                //     */
                //     node->left_factor->token =
                //         Token(node->left_factor->token.get_token_type(),
                //               std::to_string(result));

                //     node->left_factor->token.print_token();

                //     std::cout << " = " << result;
                // }
                // if (node->op.get_token_type() == T_MINUS)
                // {
                //     auto result =
                //         std::stoi(node->left_factor->token.get_token_value()) -
                //         std::stoi(node->right_factor->token.get_token_value());

                //     /* store result in left factor of current node
                //        next node will  read from this token and use it
                //     */
                //     node->left_factor->token =
                //         Token(node->left_factor->token.get_token_type(),
                //               std::to_string(result));

                //     std::cout << " = " << result;
                // }
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }
    std::cout << "<term> "
              << tabs << " EXIT INTERPRET INORDER <term>\n"
              << std::endl;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::run_parser()
{

    /* if its an identifier, get the arithmetic expression value  */
    if (current_token.get_token_type() == T_IDENTIFIER)
    {
        
        /* get variable name */
        auto variable_name = current_token.get_token_value();
        advance_current_token_index();

        /* get equals sign */
        advance_current_token_index();

        /* look for new expression */
        auto variable_value = this->expression();

        /* sometimes its null sometimes isnt, this makes it certain */
        if (variable_value->right_term == nullptr)
        {
            variable_value->left_binary_expression = nullptr;
        }

        std::cout << "has a right? " << (variable_value->right_term != nullptr) << std::endl;
        std::cout << "has a left? " << (variable_value->left_term != nullptr) << std::endl;

        interpret_expression_in_order(variable_value, 1, (variable_value->right_term != nullptr));

        std::cout << "variable name: " << variable_name
                  << std::endl;

        std::cout << "variable token type: "
                  << variable_value->left_term->left_factor->token.get_token_type()
                  << std::endl;

        std::cout << "variable token value: "
                  << variable_value->left_term->left_factor->token.get_token_value()
                  << std::endl;

        /* insert in variable map */
        symbol_table->insert(
            std::pair<std::string, Token>(
                variable_name,
                Token(
                    variable_value->left_term->left_factor->token.get_token_type(),
                    variable_value->left_term->left_factor->token.get_token_value())));
    }
    else
    {
        /* for arithmetic_expressions without identifiers */
        head_ast = this->expression();

        /* sometimes its null sometimes isnt, this makes it certain */
        if (head_ast->right_term == nullptr)
        {
            head_ast->left_binary_expression = nullptr;
        }

        std::cout << "has a right? " << (head_ast->right_term != nullptr) << std::endl;
        std::cout << "has a left? " << (head_ast->left_term != nullptr) << std::endl;

        interpret_expression_in_order(head_ast, 1, (head_ast->right_term != nullptr));

        std::cout << "head top node: " << head_ast->left_term->left_factor->token.get_token_type()
                  << " " << head_ast->left_term->left_factor->token.get_token_value()
                  << std::endl;
    }
}
/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::print_expression_in_order(ExpressionNode *node)
{
    std::cout << "\n<expr> ENTER PRINT INORDER <expr>\n"
              << std::endl;

    if (node == nullptr)
        return;
    else
    {
        /* traverse left side of the tree */
        if (node->left_term)
        {
            std::cout << "\n\t<expr> ENTER INORDER LEFT TERM <expr>\n\n";
            print_term_in_order(node->left_term, 1);
        }

        /* print operator to use */
        std::cout << " OP -> \t\t\t "
                  << node->op.get_token_value() << " " << std::endl;

        /* traverse right side of the tree */
        if (node->right_term)
        {
            std::cout << "\n\t<expr> ENTER INORDER RIGHT TERM <expr>\n\n";

            print_term_in_order(node->right_term, 1);
        }
    }

    std::cout << "\n<expr> EXIT PRINT INORDER <expr>\n"
              << std::endl;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::print_term_in_order(TermNode *node, int tabs)
{
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }

    std::cout << "<term> "
              << tabs << " ENTER PRINT INORDER  <term>\n"
              << std::endl;

    if (node == nullptr)
    {
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        std::cout << "End of Tree";
        std::cout << std::endl
                  << std::endl;

        return;
    }
    else
    {

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }

        if (node->left_factor)
            std::cout << "(" << node->left_factor->token.get_token_type()
                      << "," << node->left_factor->token.get_token_value()
                      << "),";

        if (node->right_factor)
            std::cout << "(" << node->right_factor->token.get_token_type()
                      << "," << node->right_factor->token.get_token_value()
                      << ")";

        std::cout << std::endl
                  << std::endl;

        /* traverse all the way down to the bottom node */
        print_term_in_order(node->left_binary_term, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 1; i++)
        {
            std::cout << "\t";
        }
        std::cout << "<term> "
                  << tabs << " EXIT PRINT INORDER <term>\n"
                  << std::endl;

        return;
    }
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */

void Parser::print_expression_pre_order(ExpressionNode *node)
{
    std::cout << "\n<expr> ENTER PRINT PREORDER <expr>\n"
              << std::endl;

    if (node == nullptr)

        return;
    else
    {
        /* traverse left side of the tree */
        if (node->left_term)
        {
            std::cout << "\n\t<expr> ENTER PREORDER LEFT TERM <expr>\n\n";
            print_term_pre_order(node->left_term, 1);
        }

        /* print operator to use */
        std::cout << " OP -> \t\t\t "
                  << node->op.get_token_value() << " " << std::endl;

        /* traverse right side of the tree */
        if (node->right_term)
        {
            std::cout << "\n\t<expr> ENTER PREORDER RIGHT TERM <expr>\n\n";
            print_term_pre_order(node->right_term, 1);
        }
    }

    std::cout << "\n<expr> EXIT PRINT PREORDER <expr>\n"
              << std::endl;
}

/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
  */
void Parser::print_term_pre_order(TermNode *node, int tabs)
{
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }

    std::cout << "<term> "
              << tabs << " ENTER PRINT PREORDER  <term>\n"
              << std::endl;

    if (node == nullptr)
    {

        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        std::cout << "End of Tree";
        std::cout << std::endl
                  << std::endl;
        return;
    }
    else
    {

        /* traverse all the way down to the bottom node */
        print_term_pre_order(node->left_binary_term, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }

        if (node->left_factor)
            std::cout << "(" << node->left_factor->token.get_token_type()
                      << "," << node->left_factor->token.get_token_value()
                      << "),";

        if (node->right_factor)
            std::cout << "(" << node->right_factor->token.get_token_type()
                      << "," << node->right_factor->token.get_token_value()
                      << ")";

        std::cout << std::endl
                  << std::endl;
        /* print nested tabs */
        for (int i = 0; i < tabs + 1; i++)
        {
            std::cout << "\t";
        }
        std::cout << "<term> "
                  << tabs << " EXIT PRINT PREORDER <term>\n"
                  << std::endl;
    }
}