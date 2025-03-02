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
visit(SyntaxNodeExpressionStatement *node)
{
    
    this->print_tabs();
    std::cout << "EXPRESSION STATEMENT ";
    node->expression->accept(this);
    std::cout << std::endl;
    
}

void ASTReferenceOutput::
visit(SyntaxNodeExpression *node)
{
    
    node->expression->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodeTerm *node)
{
    
    node->left->accept(this);
    
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_ADDITION: std::cout << " + "; break;
        case OperationType::OPERATION_TYPE_SUBTRACTION: std::cout << " - "; break;
        default: NOREACH("Invalid operation type");
    }
    
    node->right->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodeFactor *node)
{
    
    node->left->accept(this);
    
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_MULTIPLICATION: std::cout << " * "; break;
        case OperationType::OPERATION_TYPE_DIVISION: std::cout << " / "; break;
        case OperationType::OPERATION_TYPE_MODULUS: std::cout << " % "; break;
        default: NOREACH("Invalid operation type");
    }
    
    node->right->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodeMagnitude *node)
{
    
    node->left->accept(this);
    
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_EXPONENT: std::cout << " ^ "; break;
        default: NOREACH("Invalid operation type");
    }
    
    node->right->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodeUnary *node)
{
    
    
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_NEGATION: std::cout << "-"; break;
        case OperationType::OPERATION_TYPE_POSITIVE: std::cout << "+"; break;
        default: NOREACH("Invalid operation type");
    }
    
    node->right->accept(this);
    
}

void ASTReferenceOutput::
visit(SyntaxNodePrimary *node)
{
    
    std::cout << node->value;
    
}

void ASTReferenceOutput::
visit(SyntaxNodeGrouping *node)
{
    
    std::cout << "(";
    node->expression->accept(this);
    std::cout << ")";
    
}