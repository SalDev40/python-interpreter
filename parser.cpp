#include "parser.h"
Parser::Parser(){};

Parser::Parser(std::vector<Token> lexer_tokens_list)
{
    current_token_index = -1;
    tokens_list = lexer_tokens_list;

    /* get a token */
    advance_current_token_index();
}

void Parser::advance_current_token_index()
{
    current_token_index++;
    if (current_token_index < tokens_list.size())
        /* get the current token */
        current_token = tokens_list[current_token_index];
}

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

    /* if the current token is an integer or decimal make a number node for AST */
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        std::cout << "\t\t\t";
        current_token.print_token();
        factor_node->token = current_token;

        /* advance token */
        advance_current_token_index();
    }

    /* if the current token is a LEFT_PAREN or RIGHT_PARENT */
    if (current_token.get_token_type() == T_LEFT_PAREN)
    {
        std::cout << "\t\t\t";
        current_token.print_token();

        /* advance token */
        advance_current_token_index();
        ExpressionNode *expr = expression();

        /* @TODO: */
        interpret_expression_in_order(expr, 1);

        if (current_token.get_token_type() == T_RIGHT_PAREN)
        {
            advance_current_token_index();

            /* @TODO: solve expression and return it as a factor
               value 
            */
            // return expr;
        }
    }

    std::cout << "\n\t\t\tExit <factor>\n"
              << std::endl;

    return factor_node;
}

TermNode *Parser::term()
{

    std::cout << "\t\tEnter <term>\n"
              << std::endl;

    /* to hold the value of a single node */
    TermNode *term_node = new TermNode();
    /* will be the case when we are nesting with 2 operands */
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

ExpressionNode *Parser::expression()
{
    std::cout << "\tEnter <expr>\n"
              << std::endl;

    ExpressionNode *expression_node = new ExpressionNode();
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

void Parser::interpret_expression_in_order(ExpressionNode *node, int tabs)
{

    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }
    std::cout << "<expr> " << tabs << " ENTER INTERPRET INORDER <expr>\n"
              << std::endl;

    std::string right_result_token;
    std::string left_result_token;
    std::string right_result;
    std::string left_result;

    // if (node->right_term == nullptr)
    if (node->right_term == nullptr)
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
        interpret_expression_in_order(node->left_binary_expression, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }

        /* traverse left side of the expression node */
        if (node->left_term)
        {
            std::cout << "\t<expr> ****** ENTER INORDER LEFT TERM ****** <expr>\n\n";

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
        std::cout << "!!!!!! TOTAL = " << stoi(left_result) << std::endl
                  << std::endl;

        /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
        delete node->left_term;
        node->left_term = new TermNode();
        node->left_term->left_factor = new FactorNode();
        node->left_term->left_factor->token =
            Token(node->left_term->left_factor->token.get_token_type(),
                  std::to_string(stoi(left_result)));
    }

    if (node->right_term && !node->left_term)
    {
        std::cout << "!!!!!! TOTAL = " << stoi(right_result) << std::endl
                  << std::endl;

        /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
        delete node->left_term;
        node->left_term = new TermNode();
        node->left_term->left_factor = new FactorNode();
        node->left_term->left_factor->token =
            Token(node->left_term->left_factor->token.get_token_type(),
                  std::to_string(stoi(right_result)));
    }

    /* add left and right terms */
    if (node->left_term && node->right_term)
    {
        if (node->op.get_token_type() == T_PLUS)
        {
            std::cout << "!!!!!! TOTAL = "
                      << stoi(left_result) + stoi(right_result) << std::endl
                      << std::endl;

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();
            node->left_term->left_factor->token =
                Token(node->left_term->left_factor->token.get_token_type(),
                      std::to_string(stoi(left_result) + stoi(right_result)));
        }
        if (node->op.get_token_type() == T_MINUS)
        {
            std::cout << "!!!!!! TOTAL = "
                      << stoi(left_result) - stoi(right_result) << std::endl
                      << std::endl;

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();
            node->left_term->left_factor->token =
                Token(node->left_term->left_factor->token.get_token_type(),
                      std::to_string(stoi(left_result) - stoi(right_result)));
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

    /* @FIXME: issue is that sometimes the left->binary even tho its empty
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

                    std::cout << " = " << result;
                }
                if (node->op.get_token_type() == T_PLUS)
                {
                    auto result =
                        std::stoi(node->left_factor->token.get_token_value()) +
                        std::stoi(node->right_factor->token.get_token_value());

                    /* store result in left factor of current node
                       next node will  read from this token and use it
                    */
                    node->left_factor->token =
                        Token(node->left_factor->token.get_token_type(),
                              std::to_string(result));

                    std::cout << " = " << result;
                }
                if (node->op.get_token_type() == T_MINUS)
                {
                    auto result =
                        std::stoi(node->left_factor->token.get_token_value()) -
                        std::stoi(node->right_factor->token.get_token_value());

                    /* store result in left factor of current node
                       next node will  read from this token and use it
                    */
                    node->left_factor->token =
                        Token(node->left_factor->token.get_token_type(),
                              std::to_string(result));

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
                    std::cout << " = " << result;
                }
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

void Parser::run_parser()
{
    // auto AST = this->expression();
    head_ast = this->expression();

    std::cout << "\n\n\n\n*****************************\n";
    std::cout << "*************************************\n";
    std::cout << "INTEPRETER: " << std::endl;
    std::cout << "*************************************\n";
    std::cout << "*************************************\n";

    interpret_expression_in_order(head_ast, 1);
    // print_expression_in_order(head_ast);
}

/* 
        PRINT METHODS

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

/* 

    PRE ORDER

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