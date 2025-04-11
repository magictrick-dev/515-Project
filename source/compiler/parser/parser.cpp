#include <compiler/parser/parser.hpp>
#include <compiler/errorhandler.hpp>

SyntaxParser::
SyntaxParser(Environment *environment, Graph *graph)
    : graph(graph), environment(environment)
{
    
}

SyntaxParser::
~SyntaxParser()
{
    
}

bool SyntaxParser::
parse(string source)
{
    
    try
    {

        if (!this->tokenizer.initialize(source.c_str()))
            throw CompilerResourceException(__LINE__, 
                source, 
                "Failed to initialize the tokenizer, filepath is invalid.");
        
        this->source_name = source;
        this->base_node = this->match_root();
        
        if (CompilerException::get_exceptions_called() != 0)
            return false;
        return true;

    }
    catch (CompilerException &exception)
    {

        std::cout << exception.what() << std::endl;
        return false;

    }

    return true;
    
}

void SyntaxParser::
accept(SyntaxNodeVisitor *visitor)
{
    
    ENSURE_PTR(visitor);
    
    this->base_node->accept(visitor);
    
}

// --- Helper Functions --------------------------------------------------------

template <typename T, typename ...Args> shared_ptr<T> SyntaxParser::
create_node(Args... args)
{
    
    shared_ptr<T> node = std::make_shared<T>(args...);
    this->nodes.push_back(node);
    return node;
    
}

TokenType SyntaxParser::
get_previous_token_type() const
{

    return this->tokenizer.get_previous_token_type();

}

TokenType SyntaxParser::
get_current_token_type() const
{

    return this->tokenizer.get_current_token_type();

}

TokenType SyntaxParser::
get_next_token_type() const
{

    return this->tokenizer.get_next_token_type();

}

bool SyntaxParser::
is_previous_token_type(TokenType type) const
{

    bool result = (this->get_previous_token_type() == type);
    return result;

}

bool SyntaxParser::
is_current_token_type(TokenType type) const
{

    bool result = (this->get_current_token_type() == type);
    return result;

}

bool SyntaxParser::
is_next_token_type(TokenType type) const
{

    bool result = (this->get_next_token_type() == type);
    return result;

}

void SyntaxParser::
consume_current_token_as(TokenType type, u64 where)
{

    if (!this->is_current_token_type(type))
    {
        throw CompilerSyntaxErrorException(where, 
            this->source_name,
            this->tokenizer.get_current_token().get_line(), 
            this->tokenizer.get_current_token().get_column(), 
            "Unexpected token found in statement: %s, expecting %s.",
            this->tokenizer.get_current_token().get_reference().c_str(),
            get_type_string(type));
    }

    this->tokenizer.shift();

}

void SyntaxParser::
synchronize_to(TokenType type)
{

    while (!this->is_current_token_type(TokenType::TOKEN_EOF))
    {

        if (this->is_current_token_type(type)) break;
        this->tokenizer.shift();

    }

}

// --- Matching Functions ------------------------------------------------------

shared_ptr<SyntaxNode> SyntaxParser::
match_root()
{
    
    shared_ptr<SyntaxNode> body_statement = this->match_body();
    
    auto root = this->create_node<SyntaxNodeRoot>();
    root->base_node = body_statement;
    return root;
    
}

shared_ptr<SyntaxNode> SyntaxParser::
match_body()
{
    
    std::vector<shared_ptr<SyntaxNode>> statements;
    while (!this->tokenizer.current_token_is(TokenType::TOKEN_EOF))
    {
        
        try
        {
            
            shared_ptr<SyntaxNode> statement = this->match_statements();
            statements.push_back(statement);

        }
        catch (CompilerException &exception)
        {
            
            // TODO(Chris): Synchronize and output error.
            std::cout << exception.what() << std::endl;
            continue;
            
        }
        
    }
    
    auto body = this->create_node<SyntaxNodeBody>();
    body->statements = statements;
    return body;

}

shared_ptr<SyntaxNode> SyntaxParser::
match_statements()
{
    
    switch (this->tokenizer.get_current_token_type())
    {

        case TokenType::TOKEN_IDENTIFIER:
        {

            Token current_token = this->tokenizer.get_current_token();

            if (current_token.get_reference() == "print")
            {

                try
                {

                    shared_ptr<SyntaxNode> print_statement = this->match_print_statement();
                    return print_statement;

                }

                catch (CompilerException &exception)
                {

                    this->synchronize_to(TokenType::TOKEN_SEMICOLON);

                }

            }

        };
        
        // NOTE(Chris): If we don't match anything, it's an expression statement.
        default:
        {
            
            try
            {
                
                shared_ptr<SyntaxNode> statement = this->match_expression_statement();
                return statement;
                
            }
            catch (CompilerException &exception)
            {
                
                // TODO(Chris): Synchronize.
                throw;
                
            }

        } break;
        
    }

    // Technically, this error would never occur.
    throw CompilerSyntaxErrorException(__LINE__, 
        this->source_name,
        this->tokenizer.get_current_token().get_line(), 
        this->tokenizer.get_current_token().get_column(), 
        "Unexpected token found in statement: %s",
        this->tokenizer.get_current_token().get_reference().c_str());
    
    NOREACH("We should never reach this point or return nullptr.");
    return nullptr;
    
}

shared_ptr<SyntaxNode> SyntaxParser::
match_print_statement()
{

    Token current_token = this->tokenizer.get_current_token();
    this->consume_current_token_as(TokenType::TOKEN_IDENTIFIER, __LINE__);
    this->consume_current_token_as(TokenType::TOKEN_LEFT_PARENTHESES, __LINE__);

    std::vector<shared_ptr<SyntaxNode>> expressions;
    while (!this->is_current_token_type(TokenType::TOKEN_EOF))
    {

        if (this->is_current_token_type(TokenType::TOKEN_RIGHT_PARENTHESES)) break;

        auto expression = this->match_expression();
        expressions.push_back(expression);

        // Whoopsies, we have a comma without an expression!
        if (this->is_current_token_type(TokenType::TOKEN_COMMA) &&
            this->is_next_token_type(TokenType::TOKEN_RIGHT_PARENTHESES))
        {

            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_current_token().get_line(), 
                this->tokenizer.get_current_token().get_column(), 
                "Expression in print statement after comma.");

        }

    }

    this->consume_current_token_as(TokenType::TOKEN_RIGHT_PARENTHESES, __LINE__);

    if (expressions.size() <= 0)
    {

        throw CompilerSyntaxErrorException(__LINE__, 
            this->source_name,
            current_token.get_line(), 
            current_token.get_column(), 
            "Print statement expected at least one valid parameter.");

    }

    this->consume_current_token_as(TokenType::TOKEN_SEMICOLON, __LINE__);

    auto print_statement = this->create_node<SyntaxNodePrintStatement>();
    print_statement->expressions = expressions;
    return print_statement;

}

shared_ptr<SyntaxNode> SyntaxParser::
match_read_statement()
{

    NOREACH("We should never reach this point or return nullptr.");
    return nullptr;

}

shared_ptr<SyntaxNode> SyntaxParser::
match_expression_statement()
{
    
    shared_ptr<SyntaxNode> expression = this->match_expression();
    
    auto statement = this->create_node<SyntaxNodeExpressionStatement>();
    statement->expression = expression;
    return statement;

}

shared_ptr<SyntaxNode> SyntaxParser::
match_expression()
{
    
    shared_ptr<SyntaxNode> expression = this->match_term();
    
    auto node = this->create_node<SyntaxNodeExpression>();
    node->expression        = expression;
    node->evaluation_type   = expression->evaluation_type;
    return node;
    

}

shared_ptr<SyntaxNode> SyntaxParser::
match_term()
{
    
    shared_ptr<SyntaxNode> left = this->match_factor();

    while  (this->tokenizer.current_token_is(TokenType::TOKEN_PLUS) || 
            this->tokenizer.current_token_is(TokenType::TOKEN_MINUS))
    {
        
        // Get the current toke type and then determine the operation.
        TokenType type = this->tokenizer.get_current_token_type();
        OperationType operation = OperationType::OPERATION_TYPE_NULL;
        switch (type)
        {
            case TokenType::TOKEN_PLUS:     operation = OperationType::OPERATION_TYPE_ADDITION; break;
            case TokenType::TOKEN_MINUS:    operation = OperationType::OPERATION_TYPE_SUBTRACTION; break;
            default: NOREACH("We should never reach this point."); break;
        }

        this->tokenizer.shift();
        
        // Get the right term.
        auto right = this->match_factor();

        if (left->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL ||
            right->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL)
        {
            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_previous_token().get_line(), 
                this->tokenizer.get_previous_token().get_column(), 
                "Unexpected string literal in expression: %s",
                this->tokenizer.get_previous_token().get_reference().c_str());
        }
        
        // Generate the node.
        auto expression = this->create_node<SyntaxNodeTerm>();
        expression->left            = left;
        expression->right           = right;
        expression->operation_type  = operation;
        expression->evaluation_type = EvaluationType::EVALUATION_TYPE_INT4;
        
        left = expression;
        
    }
    
    return left;
    
}

shared_ptr<SyntaxNode> SyntaxParser::
match_factor()
{

    shared_ptr<SyntaxNode> left = this->match_magnitude();
    
    while  (this->tokenizer.current_token_is(TokenType::TOKEN_MULTIPLY) || 
            this->tokenizer.current_token_is(TokenType::TOKEN_DIVIDE) || 
            this->tokenizer.current_token_is(TokenType::TOKEN_IDENTIFIER))
    {
        
        // Get the current toke type and then determine the operation.
        TokenType type = this->tokenizer.get_current_token_type();
        OperationType operation = OperationType::OPERATION_TYPE_NULL;
        switch (type)
        {
            case TokenType::TOKEN_MULTIPLY: operation = OperationType::OPERATION_TYPE_MULTIPLICATION; break;
            case TokenType::TOKEN_DIVIDE:   operation = OperationType::OPERATION_TYPE_DIVISION; break;
            case TokenType::TOKEN_IDENTIFIER:
            {
                
                if (this->tokenizer.current_token_is(TokenType::TOKEN_IDENTIFIER, "mod"))
                {
                    
                    operation = OperationType::OPERATION_TYPE_MODULUS;
                    
                }
                else
                {
                    
                    throw CompilerSyntaxErrorException(__LINE__, 
                        this->source_name,
                        this->tokenizer.get_current_token().get_line(), 
                        this->tokenizer.get_current_token().get_column(), 
                        "Unexpected identifier encountered in expression: %s",
                        this->tokenizer.get_current_token().get_reference().c_str());
                    
                }
                
            } break;

            default: NOREACH("We should never reach this point."); break;
        }

        this->tokenizer.shift();
        
        // Get the right term.
        auto right = this->match_magnitude();

        if (left->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL ||
            right->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL)
        {
            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_previous_token().get_line(), 
                this->tokenizer.get_previous_token().get_column(), 
                "Unexpected string literal in expression: %s",
                this->tokenizer.get_previous_token().get_reference().c_str());
        }
        
        // Generate the node.
        auto term = this->create_node<SyntaxNodeFactor>();
        term->left            = left;
        term->right           = right;
        term->operation_type  = operation;
        term->evaluation_type = EvaluationType::EVALUATION_TYPE_INT4;
        
        left = term;
        
    }
    
    return left;
    
}

shared_ptr<SyntaxNode> SyntaxParser::
match_magnitude()
{

    shared_ptr<SyntaxNode> left = this->match_unary();
    
    while (this->tokenizer.current_token_is(TokenType::TOKEN_EXPONENT))
    {
        
        this->tokenizer.shift();
        
        // Get the right term.
        auto right = this->match_unary();
        
        if (left->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL ||
            right->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL)
        {
            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_previous_token().get_line(), 
                this->tokenizer.get_previous_token().get_column(), 
                "Unexpected string literal in expression: %s",
                this->tokenizer.get_previous_token().get_reference().c_str());
        }
        
        // Generate the node.
        auto factor = this->create_node<SyntaxNodeMagnitude>();
        factor->left                = left;
        factor->right               = right;
        factor->operation_type      = OperationType::OPERATION_TYPE_EXPONENT;
        factor->evaluation_type     = EvaluationType::EVALUATION_TYPE_INT4;
        
        left = factor;
        
    }
    
    return left;
   

}

shared_ptr<SyntaxNode> SyntaxParser::
match_unary()
{

    if (this->tokenizer.current_token_is(TokenType::TOKEN_MINUS) ||
        this->tokenizer.current_token_is(TokenType::TOKEN_PLUS))
    {
        
        TokenType type = this->tokenizer.get_current_token_type();
        OperationType operation = OperationType::OPERATION_TYPE_NULL;
        switch (type)
        {
            case TokenType::TOKEN_MINUS:    operation = OperationType::OPERATION_TYPE_NEGATION; break;
            case TokenType::TOKEN_PLUS:     operation = OperationType::OPERATION_TYPE_POSITIVE; break;
            default: NOREACH("We should never reach this point."); break;
        }
        
        this->tokenizer.shift();
        
        auto primary = this->match_primary();

        if (primary->evaluation_type == EvaluationType::EVALUATION_TYPE_STRING_LITERAL)
        {
            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_previous_token().get_line(), 
                this->tokenizer.get_previous_token().get_column(), 
                "Unexpected string literal in expression: %s",
                this->tokenizer.get_previous_token().get_reference().c_str());

        }
        
        auto unary = this->create_node<SyntaxNodeUnary>();
        unary->operation_type   = operation;
        unary->right            = primary;
        unary->evaluation_type  = primary->evaluation_type;
        
        return unary;
        
    }
    
    auto primary = this->match_primary();
    return primary;

}

shared_ptr<SyntaxNode> SyntaxParser::
match_primary()
{
    
    
    if (this->tokenizer.current_token_is(TokenType::TOKEN_INTEGER) ||
        this->tokenizer.current_token_is(TokenType::TOKEN_REAL) ||
        this->tokenizer.current_token_is(TokenType::TOKEN_STRING))
    {
        
        Token token = this->tokenizer.get_current_token();
        this->tokenizer.shift();
        
        PrimaryType primary_type = PrimaryType::PRIMARY_TYPE_NULL;
        EvaluationType evaluation_type = EvaluationType::EVALUATION_TYPE_NULL;
        switch (token.get_type())
        {

            case TokenType::TOKEN_INTEGER: 
            {

                primary_type = PrimaryType::PRIMARY_TYPE_INTEGER;
                evaluation_type = EvaluationType::EVALUATION_TYPE_INT4;


            } break;

            case TokenType::TOKEN_REAL:    
            {

                primary_type = PrimaryType::PRIMARY_TYPE_FLOAT;
                evaluation_type = EvaluationType::EVALUATION_TYPE_FLOAT;

            } break;

            case TokenType::TOKEN_STRING:
            {

                primary_type = PrimaryType::PRIMARY_TYPE_STRING;
                evaluation_type = EvaluationType::EVALUATION_TYPE_STRING_LITERAL;

            }

            default: NOREACH("We should never reach this point."); break;
        }

        auto node = this->create_node<SyntaxNodePrimary>();
        node->value             = token.get_reference();
        node->primary_type      = primary_type;
        node->evaluation_type   = evaluation_type;
        return node;
        
    }
    
   
    else if (this->tokenizer.current_token_is(TokenType::TOKEN_LEFT_PARENTHESES))
    {
        
        this->tokenizer.shift();
        
        auto expression = this->match_expression();
        
        if (!this->tokenizer.current_token_is(TokenType::TOKEN_RIGHT_PARENTHESES))
        {
            
            this->tokenizer.shift();
            throw CompilerSyntaxErrorException(__LINE__, 
                this->source_name,
                this->tokenizer.get_previous_token().get_line(), 
                this->tokenizer.get_previous_token().get_column(), 
                "Expected right parenthesis in expression, encountered: %s",
                this->tokenizer.get_previous_token().get_reference().c_str());
            
        }
        
        this->tokenizer.shift();
        return expression;
        
    }
    
    this->tokenizer.shift();
    throw CompilerSyntaxErrorException(__LINE__, 
        this->source_name,
        this->tokenizer.get_previous_token().get_line(), 
        this->tokenizer.get_previous_token().get_column(), 
        "Unexpected token encountered in expression, encountered: %s",
        this->tokenizer.get_previous_token().get_reference().c_str());
    
    NOREACH("We should never reach this point.");
    return nullptr;
    
}
