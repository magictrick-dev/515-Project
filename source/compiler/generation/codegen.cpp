#include <compiler/generation/codegen.hpp>

CodeGenerator::
CodeGenerator()
{
    
}

CodeGenerator::
~CodeGenerator()
{
    
}

void CodeGenerator::
visit(SyntaxNodeRoot *node)
{

    node->base_node->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeBody *node)
{

    for (auto &statement : node->statements)
    {
        
        statement->accept(this);
        
    }

}

void CodeGenerator::
visit(SyntaxNodeExpressionStatement *node)
{

    node->expression->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeExpression *node)
{


}

void CodeGenerator::
visit(SyntaxNodeTerm *node)
{

}

void CodeGenerator::
visit(SyntaxNodeFactor *node)
{

}

void CodeGenerator::
visit(SyntaxNodeMagnitude *node)
{

}

void CodeGenerator::
visit(SyntaxNodeUnary *node)
{

}

void CodeGenerator::
visit(SyntaxNodePrimary *node)
{

}

void CodeGenerator::
visit(SyntaxNodeGrouping *node)
{

}