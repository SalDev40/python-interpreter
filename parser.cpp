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

    // std::cout << "fn: " << factor_node->token.get_token_type();

    return factor_node;
}

TermNode *Parser::term()
{

    std::cout << "\t\tEnter <term>\n"
              << std::endl;

    TermNode *term_node = new TermNode();
    term_node->left_factor = nullptr;
    term_node->right_factor = nullptr;
    term_node->is_binary_op = false;
    term_node->op = Token();

    /* will be the case when we are nesting with 2 operands */
    term_node->left_binary_term = new TermNode();
    term_node->left_binary_term->left_factor = nullptr;
    term_node->left_binary_term->right_factor = nullptr;
    term_node->left_binary_term->is_binary_op = false;
    term_node->left_binary_term->op = Token();

    /* get left factor */
    term_node->left_factor = factor();
    term_node->left_binary_term->left_factor = term_node->left_factor;

    /* flag to see if we have any nesting of factors */
    int i = 0;

    /* if the token is multiply or divide */
    while (current_token.get_token_type() == T_MULTIPLY ||
           current_token.get_token_type() == T_DIVIDE)
    {
        i++;
        std::cout << "\t\t --------> ";
        current_token.print_token();

        term_node->op = current_token;

        /* advance to next token */
        advance_current_token_index();

        /* get right factor */
        term_node->right_factor = factor();

        /* re assign the left node of term node to be a binary factor node */
        term_node->left_binary_term = new TermNode(
            term_node->left_binary_term,
            term_node->left_binary_term->left_factor,
            term_node->right_factor,
            true,
            term_node->op);
    }

    /* drop first node since it duplicates */
    term_node->left_binary_term = term_node->left_binary_term->left_binary_term;

    /* if nesting occured then we must traverse the entire tree
       starting from left
   */

    if (i > 0)
        term_node->is_binary_op = true;

    std::cout << "\n\t\tExit <term>\n"
              << std::endl;

    return term_node;
}

ExpressionNode *Parser::expression()
{
    std::cout << "\tEnter <expr>\n"
              << std::endl;

    ExpressionNode *expression_node = new ExpressionNode();
    expression_node->left_term = nullptr;
    expression_node->right_term = nullptr;
    expression_node->is_binary_op = false;
    expression_node->op = Token();

    expression_node->left_binary_expression = new ExpressionNode();
    expression_node->left_binary_expression->left_term = nullptr;
    expression_node->left_binary_expression->right_term = nullptr;
    expression_node->is_binary_op = false;
    expression_node->op = Token();

    /* get left term */
    expression_node->left_term = term();
    expression_node->left_binary_expression->left_term = expression_node->left_term;

    /* flag to see if we have any nesting */
    int i = 0;

    /* if the token is plus or minus */
    while (
        current_token.get_token_type() == T_PLUS ||
        current_token.get_token_type() == T_MINUS)
    {
        i++;
        std::cout << "\t -------> ";
        current_token.print_token();

        expression_node->op = current_token;

        /* advance to next token */
        advance_current_token_index();

        /* get right term */
        expression_node->right_term = term();

        /* @TODO: issue of overwritting previous right side node */

        /* re assign the left node of expression node to be a binary term node */
        expression_node->left_binary_expression = new ExpressionNode(
            expression_node->left_binary_expression,
            expression_node->left_binary_expression->left_term,
            expression_node->right_term,
            true,
            expression_node->op);
    }

    /* if nesting occured then we must traverse the entire tree
       starting from left
   */

    if (i > 0)
        expression_node->is_binary_op = true;

    std::cout << "\n\tExit <expr>\n"
              << std::endl;

    return expression_node;
}

void Parser::run_parser()
{
    // auto AST = this->expression();
    head_ast = this->expression();

    std::cout << "\n\n\n\n"
              << std::endl;
    print_expression_in_order(head_ast);
}

void Parser::print_expression_in_order(ExpressionNode *node)
{
    std::cout << "\n<expr> ENTER INORDER <expr>\n"
              << std::endl;

    std::string right_result_token;
    std::string left_result_token;
    std::string right_result;
    std::string left_result;

    if (node == nullptr)
        return;
    else
    {
        /* traverse left side of the tree */
        if (node->left_term)
        {
            /* if its a single factor term node */
            if (!node->left_term->is_binary_op)
            {

                /* just print  its left node value */
                std::cout << "\n\t<expr> ENTER INORDER LEFT TERM <expr>\n\n";

                std::cout << "\t\t"
                          << node->left_term->left_factor->token.get_token_value();

                left_result = node->left_term->left_factor->token.get_token_value();
                left_result_token = node->left_term->left_factor->token.get_token_type();

                std::cout << "\n\t\t LEFT RESULT = "
                          << left_result;

                std::cout << "\n\n\t<expr> EXIT INORDER LEFT TERM <expr>\n\n";
            }
            else
            {
                std::cout << "\n\t<expr> ENTER INORDER LEFT TERM <expr>\n\n";
                print_term_in_order(node->left_term, 1);

                left_result = node->left_term->left_factor->token.get_token_value();
                left_result_token = node->left_term->left_factor->token.get_token_type();

                std::cout << "\n\t\t LEFT RESULT = "
                          << left_result;

                std::cout << "\n\n\t<expr> EXIT INORDER LEFT TERM <expr>\n\n";
            }
        }

        /* print operator */
        std::cout << " OP -> \t\t\t "
                  << node->op.get_token_value() << " " << std::endl;

        /* traverse right side of the tree */
        if (node->right_term)
        {
            /* if its a single factor term node */
            if (!node->right_term->is_binary_op)
            {
                /* just print  its left node value */
                std::cout << "\n\t<expr> ENTER INORDER RIGHT TERM <expr>\n\n";

                std::cout << "\t\t"
                          << node->right_term->left_factor->token.get_token_value();

                right_result = node->right_term->left_factor->token.get_token_value();
                right_result_token = node->right_term->left_factor->token.get_token_type();

                std::cout << "\n\t\t RIGHT RESULT = "
                          << right_result;

                std::cout << "\n\n\t<expr> EXIT INORDER RIGHT TERM <expr>\n\n";
            }
            else
            {
                std::cout << "\n\t<expr> ENTER INORDER RIGHT TERM <expr>\n\n";
                print_term_in_order(node->right_term, 1);

                right_result = node->right_term->left_factor->token.get_token_value();
                right_result_token = node->right_term->left_factor->token.get_token_type();

                std::cout << "\n\t\t RIGHT RESULT = "
                          << right_result;

                std::cout << "\n\n\t<expr> EXIT INORDER RIGHT TERM <expr>\n\n";
            }
        }
    }

    /* calculate the final result */
    if (node->op.get_token_type() == T_DIVIDE)
    {
        std::cout << " TOTAL = " << stoi(left_result) / stoi(right_result);
    }
    if (node->op.get_token_type() == T_MULTIPLY)
    {
        std::cout << " TOTAL = " << stoi(left_result) * stoi(right_result);
    }
    if (node->op.get_token_type() == T_PLUS)
    {
        std::cout << " TOTAL = " << stoi(left_result) + stoi(right_result);
    }
    if (node->op.get_token_type() == T_MINUS)
    {
        std::cout << " TOTAL = " << stoi(left_result) - stoi(right_result);
    }

    std::cout << "\n<expr> EXIT INORDER <expr>\n"
              << std::endl;
}

/* @TODO: crashes after a certain amount of numbers */
void Parser::print_term_in_order(TermNode *node, int tabs)
{
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }

    std::cout << "<term> " << tabs << " ENTER INORDER  <term>\n"
              << std::endl;

    // std::cout << "(" << node->left_factor->token.get_token_type()
    //           << "," << node->left_factor->token.get_token_value()
    //           << ")";
    // std::cout << "(" << node->right_factor->token.get_token_type()
    //           << "," << node->right_factor->token.get_token_value()
    //           << ")";
    if (node == nullptr)
        return;
    else
    {

        print_term_in_order(node->left_binary_term, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        if (node)
        {
            if (node->left_factor)
                std::cout << "(" << node->left_factor->token.get_token_type()
                          << "," << node->left_factor->token.get_token_value()
                          << ")";

            if (node->right_factor)
                std::cout << "(" << node->right_factor->token.get_token_type()
                          << "," << node->right_factor->token.get_token_value()
                          << ")";

            /* perform  mathematical operation on node */
            if (node->left_factor && node->right_factor)
            {
                /* find data type */
                if (node->left_factor->token.get_token_type() == T_INT &&
                    node->right_factor->token.get_token_type() == T_INT)
                {
                    if (node->op.get_token_type() == T_MULTIPLY)
                    {
                        auto result = std::stoi(node->left_factor->token.get_token_value()) *
                                      std::stoi(node->right_factor->token.get_token_value());

                        /* store result in left factor of current node
                       next node will  read from this token and use it
                    */

                        node->left_factor->token = Token(node->left_factor->token.get_token_type(),
                                                         std::to_string(result));

                        std::cout << " = " << result;
                    }
                    if (node->op.get_token_type() == T_PLUS)
                    {
                        auto result = std::stoi(node->left_factor->token.get_token_value()) +
                                      std::stoi(node->right_factor->token.get_token_value());

                        node->left_factor->token = Token(node->left_factor->token.get_token_type(),
                                                         std::to_string(result));
                        std::cout << " = " << result;
                    }
                    if (node->op.get_token_type() == T_MINUS)
                    {
                        auto result = std::stoi(node->left_factor->token.get_token_value()) -
                                      std::stoi(node->right_factor->token.get_token_value());

                        node->left_factor->token = Token(node->left_factor->token.get_token_type(),
                                                         std::to_string(result));
                        std::cout << " = " << result;
                    }
                    if (node->op.get_token_type() == T_DIVIDE)
                    {
                        auto result = std::stoi(node->left_factor->token.get_token_value()) /
                                      std::stoi(node->right_factor->token.get_token_value());

                        node->left_factor->token = Token(node->left_factor->token.get_token_type(),
                                                         std::to_string(result));
                        std::cout << " = " << result;
                    }
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
    std::cout << "<term> " << tabs << " EXIT INORDER <term>\n"
              << std::endl;
}
