#include "parser.h"

Parser::Parser(){};

Parser::Parser(std::vector<Token> *lexer_tokens_list,
               std::unordered_map<std::string, Token> *lexer_symbol_table,
               bool *is_condition_t,
               bool *should_do_else_s,
               bool *should_do_elif_s)

{
    current_token_index = -1;
    tokens_list = lexer_tokens_list;
    symbol_table = lexer_symbol_table;

    should_do_if_statement = is_condition_t;
    should_do_else_statement = should_do_else_s;
    is_if_done = should_do_elif_s;

    /* get a token */
    advance_current_token_index();
}

/**
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
 */

FactorNode *Parser::factor()
{
    FactorNode *factor_node = new FactorNode();
    factor_node->token = Token();

    /* if its a variable, get its value */
    if (current_token.get_token_type() == T_IDENTIFIER)
    {
        auto variable_data = symbol_table->find(current_token.get_token_value());
        if (variable_data == symbol_table->end())
        {
            /* variable doesnt exist, undefined error */
            std::cout << "error\n"
                      << std::endl;
            exit(-1);
        }

        /* if variable is a list and we are trying to access its elements */
        if (variable_data->second.get_token_type() == T_LIST &&
            (*tokens_list)[current_token_index + 1].get_token_type() ==
                T_LEFT_BRACKET)
        {
            //left bracket
            advance_current_token_index();

            std::string list_value = variable_data->second.get_token_value();

            //start index
            advance_current_token_index();

            /* get the start index of where to copy */
            int start_index = stoi(current_token.get_token_value());

            //get the entire list remove the commas
            std::vector<std::string> list_to_access;
            for (size_t i = 0; i < list_value.length(); i++)
            {
                std::string element = "";

                while (list_value[i] != ',' && i < list_value.length())
                {
                    element += list_value[i];
                    i++;
                }
                list_to_access.push_back(element);
            }

            /* stop index in list we are accessing */
            int stop_index;

            /* if just retrieving element or copying multi elements */
            bool one_element_access = false;

            //either colon or right bracket
            advance_current_token_index();

            /* if only one index to get element from */
            if (current_token.get_token_type() == T_RIGHT_BRACKET)
            {
                one_element_access = true;
                stop_index = -1;
            }
            /* if slicing into list */
            else if (current_token.get_token_type() == T_COLON)
            {

                /* check if token after colon is an integer */
                advance_current_token_index();

                /* get the number after the colon as the stop index  */
                if (current_token.get_token_type() == T_INT)
                {
                    //dont wanna copy last element but everything up to it
                    stop_index = stoi(current_token.get_token_value());
                }
                else
                {
                    /* if token after the colon is a bracket
                        we are copying entire remainder of list */

                    /* throw syntax error no right bracket */
                    if (current_token.get_token_type() != T_RIGHT_BRACKET)
                    {
                        std::cout << "error\n";
                        exit(-1);
                    }
                    //get everything from the remainder of the list
                    stop_index = list_to_access.size();
                }
            }

            //get that one value from the list
            if (one_element_access)
            {
                /* invalid list out of bound */
                if (start_index > (int)list_to_access.size() - 1)
                {
                    factor_node->token = Token(T_STRING, "error");
                }
                else
                {
                    auto value_in_list = list_to_access[start_index];

                    std::string value_to_save_in_token = value_in_list;

                    /* store an integer token */
                    if (std::isdigit(value_in_list[0]))
                    {
                        factor_node->token = Token(T_INT, value_to_save_in_token);
                    }
                    else
                    {
                        /* store a string token */
                        factor_node->token = Token(T_STRING, value_to_save_in_token);
                    }
                }
            }

            //copy rest of the list
            if (!one_element_access)
            {
                std::string copied_list = "";
                for (int i = start_index; i < stop_index; i++)
                {
                    copied_list += list_to_access[i];
                    copied_list += ",";
                }
                copied_list.pop_back(); //remove last comma
                factor_node->token = Token(T_LIST, copied_list);
            }
        }
        else
        {
            /* its just an identifier not list access */
            factor_node->token =
                Token(variable_data->second.get_token_type(),
                      variable_data->second.get_token_value());
        }

        advance_current_token_index();
    }

    /* if the current token is an integer or decimal make a number node for AST */
    if (current_token.get_token_type() == T_INT ||
        current_token.get_token_type() == T_DECIMAL)
    {
        factor_node->token = current_token;
        advance_current_token_index();
    }

    /* string type */
    if (current_token.get_token_type() == T_STRING)
    {
        factor_node->token = current_token;
        advance_current_token_index();
    }

    /* if the current token is a LEFT_PAREN or RIGHT_PARENT */
    if (current_token.get_token_type() == T_LEFT_PAREN)
    {
        advance_current_token_index();

        /* solve inner expression and save result value */
        ExpressionNode *expr = expression();
        interpret_expression_in_order(expr, 1, (expr->right_term != nullptr));

        /* if we get right paren, inner expression is done */
        if (current_token.get_token_type() == T_RIGHT_PAREN)
        {
            /* advance past the inner expression right paren */
            advance_current_token_index();
            /* return solved inner expression token  */
            factor_node->token = expr->left_term->left_factor->token;
        }
        else
        {
            /* throw syntax error if we dont got a right paren */
            std::cout << "error\n";
            exit(-1);
        }
    }

    /* LIST TYPE */
    if (current_token.get_token_type() == T_LEFT_BRACKET)
    {
        advance_current_token_index();

        /* store all list elements  */
        std::string list_elements = "";
        while (current_token.get_token_type() !=
               T_RIGHT_BRACKET)
        {
            if (current_token_index == tokens_list->size())
            {
                /* no right bracket == syntax error */
                std::cout << "error\n";
                exit(-1);
            }

            list_elements += current_token.get_token_value();
            advance_current_token_index();
        }

        factor_node->token = Token(T_LIST,
                                   list_elements);
    }

    return factor_node;
}

/**
 * 
 * 
 * 
 * 
 */

TermNode *Parser::term()
{

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

    return term_node;
}
/**
 * 
 * 
 * 
 * 
 */

ExpressionNode *Parser::expression()
{
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

    return expression_node;
}

/**
 * 
 * 
 * 
 * 
 */

void Parser::interpret_expression_in_order(ExpressionNode *node,
                                           int tabs,
                                           bool has_a_right_initial_tree)
{

    /* to store results from terms, will be used in previous recursive expressions */
    std::string right_result_token;
    std::string left_result_token;
    std::string right_result;
    std::string left_result;

    /* if it has a right tree recurse all the way down to second to last term */
    if (has_a_right_initial_tree && node->right_term == nullptr)
    {
        return;
    }

    /* if it has no right tree, loop all the way recursively to the bottom */
    else if (!has_a_right_initial_tree && node == nullptr)
    {
        return;
    }
    else
    {
        /* traverse all expressions till you get to last expression of terms */
        interpret_expression_in_order(node->left_binary_expression,
                                      tabs + 1,
                                      has_a_right_initial_tree);

        /* traverse left side of the expression node */
        if (node->left_term)
        {

            /* traverse the left term tree */
            interpret_term_in_order(node->left_term, tabs + 3);

            /* get the final left result value from the top node */
            left_result =
                node->left_term->left_factor->token.get_token_value();

            left_result_token =
                node->left_term->left_factor->token.get_token_type();
        }

        /* traverse right side of the expression node */
        if (node->right_term)
        {
            /* traverse the right term tree */
            interpret_term_in_order(node->right_term, tabs + 3);

            right_result = node->right_term->left_factor->token.get_token_value();
            right_result_token = node->right_term->left_factor->token.get_token_type();
        }

        /* calculate the final result and store it in left node of the current node
           next expression before it will read from it recursively
        */

        // if (current_token.get_token_type() == T_RIGHT_PAREN)
        // {
        // }

        /* remove last token in list when the advancing token doesnt remove it */

        bool is_list_concat = true;
        if (current_token.get_token_type() == T_RIGHT_BRACKET)
        {
            auto left_bracket = (*tokens_list)[current_token_index - 3].get_token_type();
            auto single_number = (*tokens_list)[current_token_index - 2].get_token_type();

            /* doing additon of values not list concatenation */
            if (left_bracket == T_LEFT_BRACKET && single_number == T_INT)
            {
                is_list_concat = false;
            }
            current_token = Token(T_LIST, "");
        }
        if (current_token.get_token_type() == T_IDENTIFIER)
        {

            auto variable_data = symbol_table->find(current_token.get_token_value());
            if (variable_data == symbol_table->end())
            {
                std::cout << "error\n";
                exit(-1);
            }

            /* set the factor value to the variable tokens value */
            current_token =
                Token(variable_data->second.get_token_type(),
                      variable_data->second.get_token_value());
        }

        /* if we only have a left term */
        if (node->left_term && !node->right_term)
        {

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();

            if (current_token.get_token_type() == T_STRING ||
                current_token.get_token_type() == T_LIST)
            {
                node->left_term->left_factor->token =
                    Token(left_result_token, left_result);
            }
            else
            {
                /* integers */
                node->left_term->left_factor->token =
                    Token(left_result_token,
                          std::to_string(stoi(left_result)));
            }

        }

        /* if we only have a right term */
        if (node->right_term && !node->left_term)
        {

            /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
            delete node->left_term;
            node->left_term = new TermNode();
            node->left_term->left_factor = new FactorNode();

            if (current_token.get_token_type() == T_STRING ||
                current_token.get_token_type() == T_LIST)
            {
                node->left_term->left_factor->token =
                    Token(left_result_token, right_result);
            }
            else
            {
                node->left_term->left_factor->token =
                    Token(right_result_token,
                          std::to_string(stoi(right_result)));
            }

        }

        /* if we have both a left and right term need to do binary operation */
        if (node->left_term && node->right_term)
        {
            if (node->op.get_token_type() == T_PLUS)
            {

                /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
                delete node->left_term;
                node->left_term = new TermNode();
                node->left_term->left_factor = new FactorNode();

                /* list element access */
                if (current_token.get_token_type() == T_LIST &&
                    !is_list_concat)
                {
                    if (!std::isdigit(right_result[0]))
                    {
                        /* string addition */
                        node->left_term->left_factor->token =
                            Token(left_result_token, left_result + right_result);
                    }
                    else
                    {
                        /* integer addition */
                        node->left_term->left_factor->token =
                            Token(left_result_token,
                                  std::to_string(stoi(left_result) +
                                                 stoi(right_result)));
                    }
                }
                else if (current_token.get_token_type() == T_LIST &&
                         is_list_concat)
                {
                    /* list concatenation */
                    node->left_term->left_factor->token =
                        Token(left_result_token,
                              left_result + "," + right_result);
                }
                else if (current_token.get_token_type() == T_STRING)
                {
                    /* string concatenation */
                    node->left_term->left_factor->token =
                        Token(left_result_token, left_result + right_result);
                }
                else
                {
                    /* integers addition */
                    node->left_term->left_factor->token =
                        Token(left_result_token,
                              std::to_string(stoi(left_result) +
                                             stoi(right_result)));
                }

            }

            if (node->op.get_token_type() == T_MINUS)
            {

                /* make the left term a new node and store the updated value in it
                will be read by previous node above it  */
                delete node->left_term;
                node->left_term = new TermNode();
                node->left_term->left_factor = new FactorNode();

                if (current_token.get_token_type() == T_STRING ||
                    current_token.get_token_type() == T_LIST)
                {
                    // std::cout << "!!!!!! L - R  TOTAL = "
                    //           << left_result - right_result << std::endl
                    //           << std::endl;
                }
                else
                {

                    node->left_term->left_factor->token =
                        Token(left_result_token,
                              std::to_string(stoi(left_result) - stoi(right_result)));
                }

            }
        }
    }
}

/**
 * 
 * 
 * 
 * 
 */

void Parser::interpret_term_in_order(TermNode *node, int tabs)
{
    /* issue is that sometimes the left->binary even tho its empty
    its not null its 0xf , but the last node right_factor is always null
    */
    if (node->right_factor == nullptr)
    {

        return;
    }
    else
    {
        /* traverse all the way down to the bottom node */
        interpret_term_in_order(node->left_binary_term, tabs + 1);

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
                }
            }
        }
    }
}

/**
 * 
 * 
 * 
 * 
 */

void Parser::run_parser(std::string current_line)
{

    /* print the value to screen */
    if (current_token.get_token_type() == T_KEYWORD_PRINT)
    {
        /* get left paren */
        advance_current_token_index();

        /* get the current value to print */
        advance_current_token_index();

       /* print variable name  */
        if ((*tokens_list)[current_token_index + 1].get_token_type() ==
                T_RIGHT_PAREN &&
            current_token.get_token_type() == T_IDENTIFIER)
        {
            /* printing a variable */
            auto variable_data = symbol_table->find(current_token.get_token_value());
            if (variable_data == symbol_table->end())
            {
                std::cout << "Variable not defined" << std::endl;
                std::cout << "error\n";
                exit(-1);
            }

            /* check if printing a list */
            if (variable_data->second.get_token_type() == T_LIST)
            {
                std::cout << "[" << variable_data->second.get_token_value()
                       << "]"
                       << std::endl;
            }
            else
            {
                std::cout << variable_data->second.get_token_value() << std::endl;
            }
        }
         /*  print string */
        else if ((*tokens_list)[current_token_index + 1].get_token_type() ==
                     T_RIGHT_PAREN &&
                 current_token.get_token_type() == T_STRING)
        {
            std::ofstream myfile;
            myfile.open("tc1.txt", std::ios_base::app);
            myfile << current_token.get_token_value() << std::endl;
            myfile.close();
        }
        else
        {
            /* print an expression */
            auto expression_to_print = expression();

            /* sometimes its null sometimes isnt, this makes it certain */
            if (expression_to_print->right_term == nullptr)
            {
                expression_to_print->left_binary_expression = nullptr;
            }

            interpret_expression_in_order(expression_to_print, 1,
                                          (expression_to_print->right_term !=
                                           nullptr));

            auto token_to_print =
                expression_to_print->left_term->left_factor->token;

            /* check if printing a list */
            if (token_to_print.get_token_type() ==
                T_LIST)
            {
                std::cout << "["
                       << token_to_print.get_token_value()
                       << "]"
                       << std::endl;
            }
            else
            {
                std::cout
                    << token_to_print.get_token_value()
                    << std::endl;
            }
        }
    }

    /**
 * 
 * 
 * 
 * 
 */

    /* if its an identifier, get the arithmetic expression value  */
    else if (current_token.get_token_type() == T_IDENTIFIER)
    {

        /* get variable name */
        auto variable_name = current_token.get_token_value();
        advance_current_token_index();

        /* get equals sign */
        advance_current_token_index();

        /* check if its a function defintion*/
        if (current_token.get_token_type() ==
            T_KEYWORD_LAMBDA)
        {

            auto end_pos_to_erase = current_line.find("lambda") + 6;
            current_line.erase(0, end_pos_to_erase);

            /* store the function body */
            symbol_table->insert(
                std::pair<std::string, Token>(
                    variable_name,
                    Token(
                        current_token.get_token_type(),
                        current_line)));
        }
        /* check if its a function call */
        else if (current_token.get_token_type() ==
                     T_IDENTIFIER &&
                 (*tokens_list)[current_token_index + 1].get_token_type() == T_LEFT_PAREN)
        {

            /* get function code */
            auto variable_data = symbol_table->find(current_token.get_token_value());
            if (variable_data == symbol_table->end())
            {
                std::cout << "error" << std::endl;
                exit(-1);
            }

            std::string function_definition = variable_data->second.get_token_value();
            std::string function_body = variable_data->second.get_token_value();
            std::string function_params = variable_data->second.get_token_value();

            /* get params */
            auto start_pos_to_erase = function_params.find(":");
            if (start_pos_to_erase == std::string::npos)
            {
                std::cout << "error\n";
                exit(-1);
            }
            function_params.erase(start_pos_to_erase, function_params.length());

            /* get definition */
            auto end_pos_to_erase = function_definition.find(":") + 1;
            if (end_pos_to_erase == std::string::npos)
            {
                std::cout << "error\n";
                exit(-1);
            }
            function_definition.erase(0, end_pos_to_erase);

            std::vector<std::string> function_params_trimmed;
            for (size_t i = 0; i < function_params.length(); i++)
            {
                std::string param_name = "";
                while (i < function_params.length() && function_params[i] != ',')
                {
                    if (function_params[i] != ' ')
                    {
                        param_name += function_params[i];
                    }
                    i++;
                };
                function_params_trimmed.push_back(param_name);
            }

            std::string function_def_trimmed = "";
            for (size_t i = 0; i < function_definition.length(); i++)
            {
                if (function_definition[i] == ' ')
                    continue;
                else
                    function_def_trimmed += function_definition[i];
            }

            function_definition = function_def_trimmed;

            /* token after left paren */
            advance_current_token_index();
            advance_current_token_index();

            std::vector<std::string> args;
            /* get function args to pass in */
            while (current_token.get_token_type() !=
                   T_RIGHT_PAREN)
            {
                if (current_token.get_token_value() != ",")
                {
                    args.push_back(current_token.get_token_value());
                }

                advance_current_token_index();
            };


            /* replace all variables with the proper values */
            for (size_t i = 0; i < function_params_trimmed.size(); i++)
            {

                auto idx_of_var = function_definition.find(
                    function_params_trimmed[i]);

                if (idx_of_var == std::string::npos)
                    break;

                auto up_to_var = function_definition;
                auto var_to_end = function_definition;

                up_to_var.erase(idx_of_var, function_definition.length());
                var_to_end.erase(0, idx_of_var + function_params_trimmed[i].length());

                function_definition = up_to_var +
                                      args[i] +
                                      var_to_end;

                advance_current_token_index();
            }

            /* if its a lambda if then solve the if  */
            auto is_if = function_definition.find("if");
            if (is_if != std::string::npos)
            {
                auto is_else = function_definition.find("else");

                if (is_else == std::string::npos)
                {
                    std::cout << "error\n";
                    exit(-1);
                }

                auto else_value = function_definition;
                auto if_value = function_definition;

                /* get proper return values for both if and else  */
                if_value.erase(is_if, function_definition.length());
                else_value.erase(0, is_else + 4); // + 4 == size of else

                /* get the condition between if and else */
                auto condition = function_definition;
                condition.erase(0, is_if + 2); // + 2 == size of if
                auto condition_else = condition.find("else");
                condition.erase(condition_else, condition.length());

                /* get the comparison operator and operands */

                /* lex condition get the tokens */
                this->tokens_list = new std::vector<Token>();
                Lexer lexer = Lexer(
                    this->tokens_list,
                    symbol_table);
                lexer.run_lexer(condition);
                current_token_index = -1;
                advance_current_token_index();

                // std::getchar();

                /* solve left and right result, then interpret results */
                std::vector<Token> *left_list = new std::vector<Token>();
                std::vector<Token> *right_list = new std::vector<Token>();

                int idx_comp_op = 0; //will be index of comparison operator

                /* get index of comparison operator */
                for (auto it = tokens_list->begin();
                     it != tokens_list->end();
                     ++it)
                {
                    idx_comp_op++;
                    if (it->get_token_type() == T_NOT_EQUAL ||
                        it->get_token_type() == T_EQUAL_EQUAL ||
                        it->get_token_type() == T_LESS_THAN ||
                        it->get_token_type() == T_LESS_THAN_EQUAL ||
                        it->get_token_type() == T_GREATER_THAN ||
                        it->get_token_type() == T_GREATER_THAN_EQUALS)
                    {
                        break;
                    }
                }

                /* copy over the expressions left and right of comparison operator */
                idx_comp_op -= 1;
                for (auto it = tokens_list->begin();
                     it != tokens_list->begin() + idx_comp_op; ++it)
                {
                    left_list->push_back(*it);
                }
                auto comparison_token = (*tokens_list)[idx_comp_op];
                for (auto it = tokens_list->begin() + idx_comp_op + 1;
                     it != tokens_list->end(); ++it)
                {
                    right_list->push_back(*it);
                }

                /* evaluate left half of the tokens list */
                tokens_list = left_list;
                current_token_index = -1;
                advance_current_token_index();
                auto left_comp_expr = expression();

                /* sometimes its null sometimes isnt, this makes it certain */
                if (left_comp_expr->right_term == nullptr)
                {
                    left_comp_expr->left_binary_expression = nullptr;
                }

                interpret_expression_in_order(left_comp_expr,
                                              1,
                                              (left_comp_expr->right_term != nullptr));

                /* evaluate right half of the tokens list */
                tokens_list = right_list;
                current_token_index = -1;
                advance_current_token_index();
                auto right_comp_expr = expression();

                /* sometimes its null sometimes isnt, this makes it certain */
                if (right_comp_expr->right_term == nullptr)
                {
                    right_comp_expr->left_binary_expression = nullptr;
                }
                interpret_expression_in_order(right_comp_expr,
                                              1,
                                              (right_comp_expr->right_term != nullptr));

                /* calculate results */
                auto left_value =
                    left_comp_expr->left_term->left_factor->token.get_token_value();
                auto left_type =
                    left_comp_expr->left_term->left_factor->token.get_token_type();

                auto right_value =
                    right_comp_expr->left_term->left_factor->token.get_token_value();
                auto right_type =
                    right_comp_expr->left_term->left_factor->token.get_token_value();

                /* make comparison check */
                bool condition_result = false;
                if (comparison_token.get_token_type() == T_NOT_EQUAL)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) != stoi(right_value);
                    else
                        condition_result = left_value != right_value;
                }
                else if (comparison_token.get_token_type() == T_EQUAL_EQUAL)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) == stoi(right_value);
                    else
                        condition_result = left_value == right_value;
                }
                else if (comparison_token.get_token_type() == T_LESS_THAN)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) < stoi(right_value);
                    else
                        condition_result = left_value < right_value;
                }
                else if (comparison_token.get_token_type() == T_LESS_THAN_EQUAL)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) <= stoi(right_value);
                    else
                        condition_result = left_value <= right_value;
                }
                else if (comparison_token.get_token_type() == T_GREATER_THAN)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) > stoi(right_value);
                    else
                        condition_result = left_value > right_value;
                }
                else if (comparison_token.get_token_type() == T_GREATER_THAN_EQUALS)
                {
                    if (left_type == T_INT)
                        condition_result = stoi(left_value) >= stoi(right_value);
                    else
                        condition_result = left_value >= right_value;
                }

                /* store the result of the condition in func def to be parsed */
                if (condition_result)
                    function_definition = if_value;
                else
                    function_definition = else_value;

                /* solve condition */
                // std::getchar();
            }

            /* lex and parse new line */
            this->tokens_list = new std::vector<Token>();
            std::string line = variable_name + "=" + function_definition;
            Lexer lexer = Lexer(
                this->tokens_list,
                symbol_table);
            lexer.run_lexer(line);
            current_token_index = -1;
            advance_current_token_index();
            this->run_parser(line);
        }

        /* for a simple variable assigment expression   */
        else
        {
            /* look for new expression */
            auto variable_value = expression();

            /* sometimes its null sometimes isnt, this makes it certain */
            if (variable_value->right_term == nullptr)
            {
                variable_value->left_binary_expression = nullptr;
            }

            interpret_expression_in_order(variable_value, 1, (variable_value->right_term != nullptr));

            /* if value exists in map, then delete it */
            auto it = symbol_table->find(variable_name);
            if (it != symbol_table->end())
            {
                symbol_table->erase(variable_name);
            }

            /* insert in variable map */
            symbol_table->insert(
                std::pair<std::string, Token>(
                    variable_name,
                    Token(
                        variable_value->left_term->left_factor->token.get_token_type(),
                        variable_value->left_term->left_factor->token.get_token_value())));
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
 * 
 * 
 * 
 */
    /* if statement */
    else if (current_token.get_token_type() == T_KEYWORD_IF)
    {

        std::vector<Token> *left_list = new std::vector<Token>();
        std::vector<Token> *right_list = new std::vector<Token>();

        int idx_comp_op = 0; //will be index of comparison operator

        /* get index of comparison operator */
        for (auto it = tokens_list->begin();
             it != tokens_list->end();
             ++it)
        {
            idx_comp_op++;
            if (it->get_token_type() == T_NOT_EQUAL ||
                it->get_token_type() == T_EQUAL_EQUAL ||
                it->get_token_type() == T_LESS_THAN ||
                it->get_token_type() == T_LESS_THAN_EQUAL ||
                it->get_token_type() == T_GREATER_THAN ||
                it->get_token_type() == T_GREATER_THAN_EQUALS)
            {
                break;
            }
        }

        /* copy over the expressions left and right of comparison operator */
        idx_comp_op -= 1;
        for (auto it = tokens_list->begin() + 1;
             it != tokens_list->begin() + idx_comp_op; ++it)
        {
            left_list->push_back(*it);
        }
        auto comparison_token = (*tokens_list)[idx_comp_op];
        for (auto it = tokens_list->begin() + idx_comp_op + 1;
             it != tokens_list->end() - 1; ++it)
        {
            right_list->push_back(*it);
        }


        /* evaluate left half of the tokens list */
        tokens_list = left_list;
        current_token_index = -1;
        advance_current_token_index();
        auto left_comp_expr = expression();

        /* sometimes its null sometimes isnt, this makes it certain */
        if (left_comp_expr->right_term == nullptr)
        {
            left_comp_expr->left_binary_expression = nullptr;
        }

        interpret_expression_in_order(left_comp_expr,
                                      1,
                                      (left_comp_expr->right_term != nullptr));

        /* evaluate right half of the tokens list */
        tokens_list = right_list;
        current_token_index = -1;
        advance_current_token_index();
        auto right_comp_expr = expression();

        /* sometimes its null sometimes isnt, this makes it certain */
        if (right_comp_expr->right_term == nullptr)
        {
            right_comp_expr->left_binary_expression = nullptr;
        }

        interpret_expression_in_order(right_comp_expr,
                                      1,
                                      (right_comp_expr->right_term != nullptr));

        /* calculate result */
        auto left_value =
            left_comp_expr->left_term->left_factor->token.get_token_value();
        auto left_type =
            left_comp_expr->left_term->left_factor->token.get_token_type();

        auto right_value =
            right_comp_expr->left_term->left_factor->token.get_token_value();
        auto right_type =
            right_comp_expr->left_term->left_factor->token.get_token_value();


        /* make comparison check */
        bool condition = false;
        if (comparison_token.get_token_type() == T_NOT_EQUAL)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) != stoi(right_value);
            else
                condition = left_value != right_value;
        }
        else if (comparison_token.get_token_type() == T_EQUAL_EQUAL)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) == stoi(right_value);
            else
                condition = left_value == right_value;
        }
        else if (comparison_token.get_token_type() == T_LESS_THAN)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) < stoi(right_value);
            else
                condition = left_value < right_value;
        }
        else if (comparison_token.get_token_type() == T_LESS_THAN_EQUAL)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) <= stoi(right_value);
            else
                condition = left_value <= right_value;
        }
        else if (comparison_token.get_token_type() == T_GREATER_THAN)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) > stoi(right_value);
            else
                condition = left_value > right_value;
        }
        else if (comparison_token.get_token_type() == T_GREATER_THAN_EQUALS)
        {
            if (left_type == T_INT)
                condition = stoi(left_value) >= stoi(right_value);
            else
                condition = left_value >= right_value;
        }


        // std::getchar();
        if (condition)
        {
            *should_do_else_statement = false;
            *is_if_done = false;
        }
        else
        {
            *should_do_else_statement = true;
            *is_if_done = true;
        }
    }

    /* else statement */
    else if (current_token.get_token_type() == T_KEYWORD_ELSE)
    {
        /* if a previous if statement passed, no need to check else block */
        if (*should_do_else_statement == false)
        {
            /* if token is else statement and we did if block mark it complete */
            *is_if_done = true;
            return;
        }
        else
        {
            *is_if_done = false;
        }
    }

    /**
 * 
 * 
 * 
 * 
 */

    else
    {
        /* for arithmetic_expressions without identifiers */
        auto arith_expression = expression();

        /* sometimes its null sometimes isnt, this makes it certain */
        if (arith_expression->right_term == nullptr)
        {
            arith_expression->left_binary_expression = nullptr;
        }

        interpret_expression_in_order(arith_expression, 1, (arith_expression->right_term != nullptr));

    }
}