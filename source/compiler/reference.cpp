#include <iostream>
#include <compiler/reference.hpp>

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
    std::cout << "PRIMARY " << evaluation_type_to_string(node->evaluation_type) << " " << node->value << std::endl;
    
}

