#include <iostream>
#include <string>
#include <compiler/reference.hpp>

static inline string 
escape_string(const string& input) {
    std::string result;
    result.reserve(input.size());

    for (char c : input) {
        switch (c) {
            case '\n': result += "\\n";     break;
            case '\t': result += "\\t";     break;
            case '\r': result += "\\r";     break;
            case '\b': result += "\\b";     break;
            case '\f': result += "\\f";     break;
            case '\v': result += "\\v";     break;
            case '\a': result += "\\a";     break;
            case '\\': result += "\\\\";    break;
            case '\0': result += "\\0";     break;

            default:
                if (std::isprint(static_cast<unsigned char>(c))) {
                    result += c;
                } else {
                    // Optional: handle non-printable characters as hex
                    char buf[5];
                    snprintf(buf, sizeof(buf), "\\x%02X", static_cast<unsigned char>(c));
                    result += buf;

                }
                break;
        }
    }

    return result;

}

ASTReferenceOutput::
ASTReferenceOutput(int tab_size)
{
    
    this->tabs = 0;
    this->tabs_size = tab_size;
    
}

ASTReferenceOutput::
~ASTReferenceOutput()
{
    
}

void ASTReferenceOutput::
push_tabs()
{
    
    this->tabs += this->tabs_size;
    
}

void ASTReferenceOutput::
pop_tabs()
{
    
    this->tabs -= this->tabs_size;
    
}

void ASTReferenceOutput::
print_tabs() const
{
    
    for (int i = 0; i < this->tabs; i++)
    {
        
        std::cout << " ";
        
    }
    
}

void ASTReferenceOutput::
visit(SyntaxNodeRoot *node)
{
    
    this->print_tabs();
    std::cout << "ROOT" << std::endl;
    
    this->push_tabs();
    node->base_node->accept(this);
    this->pop_tabs();

    this->print_tabs();
    std::cout << "END ROOT" << std::endl;
    
}

void ASTReferenceOutput::
visit(SyntaxNodeBody *node)
{
    
    this->print_tabs();
    std::cout << "BODY" << std::endl;
    
    this->push_tabs();
    for (auto &statement : node->statements)
    {
        
        statement->accept(this);
        
    }
    this->pop_tabs();
    
    this->print_tabs();
    std::cout << "END BODY" << std::endl;
    
}

void ASTReferenceOutput::
visit(SyntaxNodeVariableStatement *node)
{

    this->print_tabs();
    std::cout << "VARIABLE STATEMENT " << node->variable_name << std::endl;

}

void ASTReferenceOutput::
visit(SyntaxNodeReadStatement *node)
{

    this->print_tabs();
    std::cout << "READ STATEMENT " << node->variable_name << std::endl;


}

void ASTReferenceOutput::
visit(SyntaxNodeConditionalStatement *node)
{

    this->print_tabs();
    std::cout << "CONDITIONAL IF STATEMENT" << std::endl;

    this->push_tabs();
    node->condition->accept(this);

    this->print_tabs();
    std::cout << "CONDITIONAL BODY" << std::endl;
    this->push_tabs();
    for (auto node : node->conditional_if)
    {
        node->accept(this);
    }
    this->pop_tabs();
    this->pop_tabs();

    if (node->conditional_else.empty() == false)
    {

        this->print_tabs();
        std::cout << "CONDITIONAL ELSE STATEMENT" << std::endl;
        this->push_tabs();
        for (auto node : node->conditional_else)
        {
            node->accept(this);
        }
        this->pop_tabs();

    }

}

void ASTReferenceOutput::
visit(SyntaxNodeWhileStatement *node)
{

    this->print_tabs();
    std::cout << "WHILE STATEMENT" << std::endl;

    this->push_tabs();
    node->condition->accept(this);

    this->print_tabs();
    std::cout << "WHILE BODY" << std::endl;

    this->push_tabs();
    for (auto node : node->statements)
    {
        node->accept(this);
    }

    this->pop_tabs();
    this->pop_tabs();

}

void ASTReferenceOutput::
visit(SyntaxNodePrintStatement *node)
{

    this->print_tabs();
    std::cout << "PRINT STATEMENT" << std::endl;

    this->push_tabs();

    for (auto expression : node->expressions)
    {

        expression->accept(this);

    }

    this->pop_tabs();

}

void ASTReferenceOutput::
visit(SyntaxNodeAssignmentStatement *node)
{
    
    this->print_tabs();
    std::cout << "ASSIGNMENT STATEMENT " << node->variable_name << std::endl;
    this->push_tabs();
    node->expression->accept(this);
    this->pop_tabs();
    
}

void ASTReferenceOutput::
visit(SyntaxNodeExpressionStatement *node)
{
    
    this->print_tabs();
    std::cout << "EXPRESSION STATEMENT ";
    std::cout << evaluation_type_to_string(node->expression->evaluation_type) << std::endl;
    this->push_tabs();
    node->expression->accept(this);
    this->pop_tabs();
    
}

void ASTReferenceOutput::
visit(SyntaxNodeExpression *node)
{
    
    node->expression->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodeLogical *node)
{

    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_LOGICAL_OR: std::cout << "OR "; break;
        case OperationType::OPERATION_TYPE_LOGICAL_AND: std::cout << "AND "; break;
        case OperationType::OPERATION_TYPE_LOGICAL_NOT: std::cout << "NOT "; break;
        default: NOREACH("Invalid operation type");
    }

    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;

    this->push_tabs();

    // Not is unary.
    if (node->operation_type != OperationType::OPERATION_TYPE_LOGICAL_NOT)
    {
        node->left->accept(this);
    }

    node->right->accept(this);

    this->pop_tabs();

}

void ASTReferenceOutput::
visit(SyntaxNodeRelational *node)
{

    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_LESS: std::cout << "LESS "; break;
        case OperationType::OPERATION_TYPE_LESS_EQUAL: std::cout << "LESS EQUALS"; break;
        case OperationType::OPERATION_TYPE_GREATER: std::cout << "GREATER "; break;
        case OperationType::OPERATION_TYPE_GREATER_EQUAL: std::cout << "GREATER EQUALS"; break;
        case OperationType::OPERATION_TYPE_EQUAL: std::cout << "EQUAL"; break;
        case OperationType::OPERATION_TYPE_NOT_EQUAL: std::cout << "NOT EQUAL"; break;
        default: NOREACH("Invalid operation type");
    }

    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;

    this->push_tabs();

    node->left->accept(this);
    node->right->accept(this);

    this->pop_tabs();

}

void ASTReferenceOutput::
visit(SyntaxNodeTerm *node)
{
    
    
    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_ADDITION: std::cout << "ADDITION "; break;
        case OperationType::OPERATION_TYPE_SUBTRACTION: std::cout << "SUBTRACTION "; break;
        default: NOREACH("Invalid operation type");
    }

    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;

    this->push_tabs();

    node->left->accept(this);
    node->right->accept(this);

    this->pop_tabs();
    
}

void ASTReferenceOutput::
visit(SyntaxNodeFactor *node)
{
    
    
    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_MULTIPLICATION: std::cout << "MULTIPLICATION "; break;
        case OperationType::OPERATION_TYPE_DIVISION: std::cout << "DIVISION "; break;
        case OperationType::OPERATION_TYPE_MODULUS: std::cout << "MODULUS "; break;
        default: NOREACH("Invalid operation type");
    }

    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;
    
    this->push_tabs();

    node->left->accept(this);
    node->right->accept(this);

    this->pop_tabs();
    
}

void ASTReferenceOutput::
visit(SyntaxNodeMagnitude *node)
{
    
    
    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_EXPONENT: std::cout << "EXPONENTIATION "; break;
        default: NOREACH("Invalid operation type");
    }
    
    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;

    this->push_tabs();

    node->left->accept(this);
    node->right->accept(this);
    
    this->pop_tabs();

}

void ASTReferenceOutput::
visit(SyntaxNodeUnary *node)
{
    
    
    this->print_tabs();
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_NEGATION: std::cout << "UNARY - "; break;
        case OperationType::OPERATION_TYPE_POSITIVE: std::cout << "UNARY + "; break;
        default: NOREACH("Invalid operation type");
    }

    std::cout << evaluation_type_to_string(node->evaluation_type) << std::endl;

    this->push_tabs();
    
    node->right->accept(this);

    this->pop_tabs();
    
}

void ASTReferenceOutput::
visit(SyntaxNodePrimary *node)
{
    
    this->print_tabs();
    std::cout << "PRIMARY " << evaluation_type_to_string(node->evaluation_type) << " " << escape_string(node->value) << std::endl;
    
}

