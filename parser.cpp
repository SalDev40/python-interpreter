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

        /* re assign the left node of term node to be a binary node */
        term_node->left_binary_term = new TermNode(
            term_node->left_binary_term,
            term_node->left_binary_term->left_factor,
            term_node->right_factor,
            true,
            term_node->op);
    }

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
                          
                std::cout << "\n\n\t<expr> EXIT INORDER LEFT TERM <expr>\n\n";
            }
            else
            {
                std::cout << "\n\t<expr> ENTER INORDER LEFT TERM <expr>\n\n";
                print_term_in_order(node->left_term, 1);
                std::cout << "\n\t<expr> EXIT INORDER LEFT TERM <expr>\n\n";
            }
        }

        /* print operator */
        std::cout << " OP -> \t\t\t " << node->op.get_token_value() << " " << std::endl;

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

                std::cout << "\n\n\t<expr> EXIT INORDER RIGHT TERM <expr>\n\n";
            }
            else
            {
                std::cout << "\n\t<expr> ENTER INORDER RIGHT TERM <expr>\n\n";
                print_term_in_order(node->right_term, 1);
                std::cout << "\n\t<expr> EXIT INORDER RIGHT TERM <expr>\n\n";
            }
        }
    }
    std::cout << "\n<expr> EXIT INORDER <expr>\n"
              << std::endl;
}

/* @TODO: maybe return previous node addition of left + right */
void Parser::print_term_in_order(TermNode *node, int tabs)
{
    /* print nested tabs */
    for (int i = 0; i < tabs + 1; i++)
    {
        std::cout << "\t";
    }

    std::cout << "<term> " << tabs << " ENTER INORDER  <term>\n"
              << std::endl;

    if (node == nullptr)
        return;
    else
    {

        /* @TODO: perform operation and store in Left_term value */

        print_term_in_order(node->left_binary_term, tabs + 1);

        /* print nested tabs */
        for (int i = 0; i < tabs + 2; i++)
        {
            std::cout << "\t";
        }
        if (node->left_factor)
            std::cout << node->left_factor->token.get_token_value() << " ";

        if (node)
            std::cout << node->op.get_token_value() << " ";

        if (node->right_factor)
            std::cout << node->right_factor->token.get_token_value() << " ";

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

// void Parser::print_factor_post_order(TermNode* node)
// {
//     if (node == NULL)
//         return;

//     // first recur on left subtree
//     print_term_post_order(node->left);

//     // then recur on right subtree
//     print_term_post_order(node->right);

//     // now deal with the node
//     std::cout << node->op.get_token_value() << " ";
// }

// #include "parser.h"
// Parser::Parser(){};

// Parser::Parser(std::vector<Token> lexer_tokens_list)
// {
//     current_token_index = -1;
//     tokens_list = lexer_tokens_list;

//     /* get a token */
//     advance_current_token_index();
// }

// void Parser::advance_current_token_index()
// {
//     current_token_index++;
//     if (current_token_index < tokens_list.size())
//         /* get the current token */
//         current_token = tokens_list[current_token_index];
// }

// NumberNode *Parser::factor()
// {
//     /* if the current token is an integer or decimal make a number node for AST */
//     if (current_token.get_token_type() == T_INT ||
//         current_token.get_token_type() == T_DECIMAL)
//     {
//         return new NumberNode(current_token);
//     }
// }

// BinaryOpFactors *Parser::term()
// {
//     /* get left factor */

//     /* @TODO: issue is left_node can be either a numbernode or binaryOpnode */
//     NumberNode *left_node = factor();

//     std::cout << "left factor -> "
//               << left_node->token.get_token_type() << std::endl;

//     /* advance to next token */
//     // advance_current_token_index();

//     std::cout << "current_token -> "
//               << current_token.get_token_type() << std::endl;

//     BinaryOpFactors *binary_operation = new BinaryOpFactors();

//     /* if the token is multiply or divide */
//     while (current_token.get_token_type() == T_MULTIPLY ||
//            current_token.get_token_type() == T_DIVIDE)
//     {

//         /* get the token */
//         Token operator_token = current_token;
//         std::cout << "operator -> "
//                   << operator_token.get_token_type() << std::endl;

//         /* advance to next token */
//         advance_current_token_index();

//         /* get right factor */
//         NumberNode *right_node = factor();
//         std::cout << "right factor -> "
//                   << right_node->token.get_token_type() << std::endl;

//         /* do a binary operation between both factors */
//         binary_operation =
//             new BinaryOpFactors(
//                 left_node,
//                 right_node,
//                 operator_token.get_token_value());
//     }

//     // std::cout << " " << binary_operation->left_node->token.get_token_type();
//     // std::cout << " " << binary_operation->left_node->token.get_token_value();

//     // std::cout << " " << binary_operation->op;

//     // std::cout << " " << binary_operation->right_node->token.get_token_type();
//     // std::cout << " " << binary_operation->right_node->token.get_token_value();

//     return binary_operation;
// }

// BinaryOpTerms Parser::expression()
// {
//     /* get left term */
//     BinaryOpFactors *left_node = term();

//     BinaryOpTerms binary_operation = BinaryOpTerms();

//     /* if the token is multiply or divide */
//     while (
//         current_token.get_token_type() == T_PLUS ||
//         current_token.get_token_type() == T_MINUS)
//     {

//         /* get the token */
//         Token operator_token = current_token;

//         /* advance to next token */
//         advance_current_token_index();

//         /* get right term */
//         BinaryOpFactors *right_node = term();

//         /* do a binary operation between both terms */
//         binary_operation =
//             BinaryOpTerms(left_node,
//                           right_node,
//                           operator_token.get_token_value());
//     }

//     return binary_operation;
// }

// void Parser::factor1()
// {
//     std::cout << "\t\t\tEnter <factor>\n"
//               << std::endl;

//     /* if the current token is an integer or decimal make a number node for AST */
//     if (current_token.get_token_type() == T_INT ||
//         current_token.get_token_type() == T_DECIMAL)
//     {
//         std::cout << "\t\t\t";
//         current_token.print_token();
//     }

//     /* advance token */
//     advance_current_token_index();

//     std::cout << "\n\t\t\tExit <factor>\n"
//               << std::endl;
// }

// void Parser::term1()
// {
//     std::cout << "\t\tEnter <term>\n"
//               << std::endl;
//     /* get left factor */
//     factor1();

//     /* if the token is multiply or divide */
//     while (current_token.get_token_type() == T_MULTIPLY ||
//            current_token.get_token_type() == T_DIVIDE)
//     {

//         std::cout << "\t\t --------> ";
//         current_token.print_token();

//         /* advance to next token */
//         advance_current_token_index();

//         /* get right factor */
//         factor1();
//     }

//     // advance_current_token_index();

//     std::cout << "\n\t\tExit <term>\n"
//               << std::endl;
// }

// void Parser::expression1()
// {
//     std::cout << "\tEnter <expr>\n"
//               << std::endl;

//     /* get left term */
//     term1();

//     /* if the token is plus or minus */
//     while (
//         current_token.get_token_type() == T_PLUS ||
//         current_token.get_token_type() == T_MINUS)
//     {

//         std::cout << "\t -------> ";
//         current_token.print_token();
//         /* advance to next token */
//         advance_current_token_index();

//         /* get right term */
//         term1();
//     }
//     std::cout << "\n\tExit <expr>\n"
//               << std::endl;
// }

// void Parser::run_parser()
// {
//     // auto AST = this->expression();
//     this->expression1();
// }
// #include "parser.h"
// Parser::Parser(){};

// Parser::Parser(std::vector<Token> lexer_tokens_list)
// {
//     current_token_index = -1;
//     tokens_list = lexer_tokens_list;

//     /* get a token */
//     advance_current_token_index();
// }

// void Parser::advance_current_token_index()
// {
//     current_token_index++;
//     if (current_token_index < tokens_list.size())
//         /* get the current token */
//         current_token = tokens_list[current_token_index];
// }

// FactorNode *Parser::factor()
// {
//     std::cout << "\t\t\tEnter <factor>\n"
//               << std::endl;

//     FactorNode *factor_node = new FactorNode();
//     factor_node->token = Token();

//     /* if the current token is an integer or decimal make a number node for AST */
//     if (current_token.get_token_type() == T_INT ||
//         current_token.get_token_type() == T_DECIMAL)
//     {
//         std::cout << "\t\t\t";
//         current_token.print_token();

//         // factor();

//         factor_node->token = current_token;
//     }

//     /* advance token */
//     advance_current_token_index();

//     std::cout << "\n\t\t\tExit <factor>\n"
//               << std::endl;

//     // std::cout << "fn: " << factor_node->token.get_token_type();

//     return factor_node;
// }

// TermNode *Parser::term()
// {

//     std::cout << "\t\tEnter <term>\n"
//               << std::endl;

//     TermNode *term_node = new TermNode();
//     term_node->left = nullptr;
//     term_node->right = nullptr;
//     term_node->is_binary_op = false;
//     term_node->op = Token();

//     /* get left factor */
//     term_node->left = factor();

//     /* if the token is multiply or divide */
//     while (current_token.get_token_type() == T_MULTIPLY ||
//            current_token.get_token_type() == T_DIVIDE)
//     {

//         std::cout << "\t\t --------> ";
//         current_token.print_token();

//         term_node->op = current_token;

//         /* advance to next token */
//         advance_current_token_index();

//         /* get right factor */
//         term_node->right = factor();
//     }

//     /* if we have a left and a right factor node  */
//     // std::cout << "\nhello\n";
//     if ((term_node->right && term_node->left) &&
//         (term_node->right->token.get_token_type() != "N/A" &&
//          term_node->left->token.get_token_type() != "N/A"))
//     {
//         term_node->is_binary_op = true;
//     }
//     // std::cout << "\nworld\n";

//     // advance_current_token_index();

//     std::cout << "\n\t\tExit <term>\n"
//               << std::endl;

//     return term_node;
// }

// ExpressionNode *Parser::expression()
// {
//     std::cout << "\tEnter <expr>\n"
//               << std::endl;

//     ExpressionNode *expression_node = new ExpressionNode();
//     expression_node->left = nullptr;
//     expression_node->right = nullptr;
//     expression_node->is_binary_op = false;
//     expression_node->op = Token();

//     /* get left term */
//     expression_node->left = term();
//     std::cout << "left expr node done: " << std::endl;

//     /* if the token is plus or minus */
//     while (
//         current_token.get_token_type() == T_PLUS ||
//         current_token.get_token_type() == T_MINUS)
//     {

//         std::cout << "\t -------> ";
//         current_token.print_token();

//         expression_node->op = current_token;

//         /* advance to next token */
//         advance_current_token_index();

//         /* get right term */
//         expression_node->right = term();
//         std::cout << "right expr node done: " << std::endl;
//     }

//     /* if we have a left and a right term node  */
//     if ((expression_node->right && expression_node->left) &&
//         (expression_node->right->op.get_token_type() != "N/A" &&
//          expression_node->left->op.get_token_type() != "N/A"))
//     {
//         expression_node->is_binary_op = true;
//     }

//     std::cout << "\n\tExit <expr>\n"
//               << std::endl;

//     return expression_node;
// }

// void Parser::run_parser()
// {
//     // auto AST = this->expression();
//     head_ast = this->expression();
// }

// // void Parser::print_post_order(ExpressionNode* node)
// // {
// //     if (node == NULL)
// //         return;

// //     // first recur on left subtree
// //     print_post_order(node->left);

// //     // then recur on right subtree
// //     print_post_order(node->right);

// //     // now deal with the node
// //     std::cout << node->op.get_token_value() << " ";
// // }

// // void Parser::print_term_post_order(TermNode* node)
// // {
// //     if (node == NULL)
// //         return;

// //     // first recur on left subtree
// //     print_term_post_order(node->left);

// //     // then recur on right subtree
// //     print_term_post_order(node->right);

// //     // now deal with the node
// //     std::cout << node->op.get_token_value() << " ";
// // }

// // void Parser::print_factor_post_order(TermNode* node)
// // {
// //     if (node == NULL)
// //         return;

// //     // first recur on left subtree
// //     print_term_post_order(node->left);

// //     // then recur on right subtree
// //     print_term_post_order(node->right);

// //     // now deal with the node
// //     std::cout << node->op.get_token_value() << " ";
// // }

// // #include "parser.h"
// // Parser::Parser(){};

// // Parser::Parser(std::vector<Token> lexer_tokens_list)
// // {
// //     current_token_index = -1;
// //     tokens_list = lexer_tokens_list;

// //     /* get a token */
// //     advance_current_token_index();
// // }

// // void Parser::advance_current_token_index()
// // {
// //     current_token_index++;
// //     if (current_token_index < tokens_list.size())
// //         /* get the current token */
// //         current_token = tokens_list[current_token_index];
// // }

// // NumberNode *Parser::factor()
// // {
// //     /* if the current token is an integer or decimal make a number node for AST */
// //     if (current_token.get_token_type() == T_INT ||
// //         current_token.get_token_type() == T_DECIMAL)
// //     {
// //         return new NumberNode(current_token);
// //     }
// // }

// // BinaryOpFactors *Parser::term()
// // {
// //     /* get left factor */

// //     /* @TODO: issue is left_node can be either a numbernode or binaryOpnode */
// //     NumberNode *left_node = factor();

// //     std::cout << "left factor -> "
// //               << left_node->token.get_token_type() << std::endl;

// //     /* advance to next token */
// //     // advance_current_token_index();

// //     std::cout << "current_token -> "
// //               << current_token.get_token_type() << std::endl;

// //     BinaryOpFactors *binary_operation = new BinaryOpFactors();

// //     /* if the token is multiply or divide */
// //     while (current_token.get_token_type() == T_MULTIPLY ||
// //            current_token.get_token_type() == T_DIVIDE)
// //     {

// //         /* get the token */
// //         Token operator_token = current_token;
// //         std::cout << "operator -> "
// //                   << operator_token.get_token_type() << std::endl;

// //         /* advance to next token */
// //         advance_current_token_index();

// //         /* get right factor */
// //         NumberNode *right_node = factor();
// //         std::cout << "right factor -> "
// //                   << right_node->token.get_token_type() << std::endl;

// //         /* do a binary operation between both factors */
// //         binary_operation =
// //             new BinaryOpFactors(
// //                 left_node,
// //                 right_node,
// //                 operator_token.get_token_value());
// //     }

// //     // std::cout << " " << binary_operation->left_node->token.get_token_type();
// //     // std::cout << " " << binary_operation->left_node->token.get_token_value();

// //     // std::cout << " " << binary_operation->op;

// //     // std::cout << " " << binary_operation->right_node->token.get_token_type();
// //     // std::cout << " " << binary_operation->right_node->token.get_token_value();

// //     return binary_operation;
// // }

// // BinaryOpTerms Parser::expression()
// // {
// //     /* get left term */
// //     BinaryOpFactors *left_node = term();

// //     BinaryOpTerms binary_operation = BinaryOpTerms();

// //     /* if the token is multiply or divide */
// //     while (
// //         current_token.get_token_type() == T_PLUS ||
// //         current_token.get_token_type() == T_MINUS)
// //     {

// //         /* get the token */
// //         Token operator_token = current_token;

// //         /* advance to next token */
// //         advance_current_token_index();

// //         /* get right term */
// //         BinaryOpFactors *right_node = term();

// //         /* do a binary operation between both terms */
// //         binary_operation =
// //             BinaryOpTerms(left_node,
// //                           right_node,
// //                           operator_token.get_token_value());
// //     }

// //     return binary_operation;
// // }

// // void Parser::factor1()
// // {
// //     std::cout << "\t\t\tEnter <factor>\n"
// //               << std::endl;

// //     /* if the current token is an integer or decimal make a number node for AST */
// //     if (current_token.get_token_type() == T_INT ||
// //         current_token.get_token_type() == T_DECIMAL)
// //     {
// //         std::cout << "\t\t\t";
// //         current_token.print_token();
// //     }

// //     /* advance token */
// //     advance_current_token_index();

// //     std::cout << "\n\t\t\tExit <factor>\n"
// //               << std::endl;
// // }

// // void Parser::term1()
// // {
// //     std::cout << "\t\tEnter <term>\n"
// //               << std::endl;
// //     /* get left factor */
// //     factor1();

// //     /* if the token is multiply or divide */
// //     while (current_token.get_token_type() == T_MULTIPLY ||
// //            current_token.get_token_type() == T_DIVIDE)
// //     {

// //         std::cout << "\t\t --------> ";
// //         current_token.print_token();

// //         /* advance to next token */
// //         advance_current_token_index();

// //         /* get right factor */
// //         factor1();
// //     }

// //     // advance_current_token_index();

// //     std::cout << "\n\t\tExit <term>\n"
// //               << std::endl;
// // }

// // void Parser::expression1()
// // {
// //     std::cout << "\tEnter <expr>\n"
// //               << std::endl;

// //     /* get left term */
// //     term1();

// //     /* if the token is plus or minus */
// //     while (
// //         current_token.get_token_type() == T_PLUS ||
// //         current_token.get_token_type() == T_MINUS)
// //     {

// //         std::cout << "\t -------> ";
// //         current_token.print_token();
// //         /* advance to next token */
// //         advance_current_token_index();

// //         /* get right term */
// //         term1();
// //     }
// //     std::cout << "\n\tExit <expr>\n"
// //               << std::endl;
// // }

// // void Parser::run_parser()
// // {
// //     // auto AST = this->expression();
// //     this->expression1();
// // }