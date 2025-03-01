#include <compiler/parser/subnodes.hpp>
#include <compiler/parser/visitor.hpp>

// --- Root Syntax Node --------------------------------------------------------

SyntaxNodeRoot::
SyntaxNodeRoot()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_ROOT;   
    this->base_node = nullptr;

}

SyntaxNodeRoot::
~SyntaxNodeRoot()
{
    
}

void SyntaxNodeRoot::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Body Syntax Node --------------------------------------------------------

SyntaxNodeBody::
SyntaxNodeBody()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_BODY;

}

SyntaxNodeBody::
~SyntaxNodeBody()
{
    
}

void SyntaxNodeBody::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Expression Statement Syntax Node -----------------------------------------

SyntaxNodeExpressionStatement::
SyntaxNodeExpressionStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_EXPRESSION_STATEMENT;
    this->expression = nullptr;

}

SyntaxNodeExpressionStatement::
~SyntaxNodeExpressionStatement()
{
    
}

void SyntaxNodeExpressionStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Expression Syntax Node --------------------------------------------------

SyntaxNodeExpression::
SyntaxNodeExpression()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_EXPRESSION;
    this->expression = nullptr;

}

SyntaxNodeExpression::
~SyntaxNodeExpression()
{
    
}

void SyntaxNodeExpression::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Term Syntax Node --------------------------------------------------------

SyntaxNodeTerm::
SyntaxNodeTerm()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_TERM;
    this->left = nullptr;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeTerm::
~SyntaxNodeTerm()
{
    
}

void SyntaxNodeTerm::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Factor Syntax Node ------------------------------------------------------

SyntaxNodeFactor::
SyntaxNodeFactor()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_FACTOR;
    this->left = nullptr;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeFactor::
~SyntaxNodeFactor()
{
    
}

void SyntaxNodeFactor::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Magnitude Syntax Node ---------------------------------------------------

SyntaxNodeMagnitude::
SyntaxNodeMagnitude()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_MAGNITUDE;
    this->left = nullptr;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeMagnitude::
~SyntaxNodeMagnitude()
{
    
}

void SyntaxNodeMagnitude::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Unary Syntax Node -------------------------------------------------------

SyntaxNodeUnary::
SyntaxNodeUnary()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_UNARY;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeUnary::
~SyntaxNodeUnary()
{
    
}

void SyntaxNodeUnary::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Primary Syntax Node -----------------------------------------------------

SyntaxNodePrimary::
SyntaxNodePrimary()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_PRIMARY;
    this->primary_type = PrimaryType::PRIMARY_TYPE_NULL;
    this->value = "";

}

SyntaxNodePrimary::
~SyntaxNodePrimary()
{
    
}

void SyntaxNodePrimary::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Grouping Syntax Node ----------------------------------------------------

SyntaxNodeGrouping::
SyntaxNodeGrouping()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_GROUPING;
    this->expression = nullptr;

}

SyntaxNodeGrouping::
~SyntaxNodeGrouping()
{
    
}

void SyntaxNodeGrouping::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}
