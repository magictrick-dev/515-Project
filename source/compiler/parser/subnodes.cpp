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

// --- Variable Statement Syntax Node ------------------------------------------

SyntaxNodeVariableStatement::
SyntaxNodeVariableStatement()
{

    this->node_type         = SyntaxNodeType::SYNTAX_NODE_TYPE_VARIABLE_STATEMENT;
    this->variable_name     = "";
    this->address           = NULL;

}

SyntaxNodeVariableStatement::
~SyntaxNodeVariableStatement()
{
    
}

void SyntaxNodeVariableStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Read Statement Syntax Node ---------------------------------------------

SyntaxNodeReadStatement::
SyntaxNodeReadStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_READ_STATEMENT;

}

SyntaxNodeReadStatement::
~SyntaxNodeReadStatement()
{
    
}

void SyntaxNodeReadStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- While Statement Syntax Node ---------------------------------------------

SyntaxNodeWhileStatement::
SyntaxNodeWhileStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_WHILE_STATEMENT;

}

SyntaxNodeWhileStatement::
~SyntaxNodeWhileStatement()
{
    
}

void SyntaxNodeWhileStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- If Statement Syntax Node ------------------------------------------------

SyntaxNodeConditionalStatement::
SyntaxNodeConditionalStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_CONDITIONAL_STATEMENT;

}

SyntaxNodeConditionalStatement::
~SyntaxNodeConditionalStatement()
{
    
}

void SyntaxNodeConditionalStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Print Statement Syntax Node ---------------------------------------------

SyntaxNodePrintStatement::
SyntaxNodePrintStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_PRINT_STATEMENT;

}

SyntaxNodePrintStatement::
~SyntaxNodePrintStatement()
{
    
}

void SyntaxNodePrintStatement::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Assignment Statement Syntax Node -----------------------------------------

SyntaxNodeAssignmentStatement::
SyntaxNodeAssignmentStatement()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_ASSIGNMENT_STATEMENT;
    this->expression = nullptr;

}

SyntaxNodeAssignmentStatement::
~SyntaxNodeAssignmentStatement()
{
    
}

void SyntaxNodeAssignmentStatement::
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

// --- Relational Syntax Node --------------------------------------------------------

SyntaxNodeRelational::
SyntaxNodeRelational()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_RELATIONAL;
    this->left = nullptr;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeRelational::
~SyntaxNodeRelational()
{
    
}

void SyntaxNodeRelational::
accept(SyntaxNodeVisitor *visitor)
{
    visitor->visit(this);
}

// --- Logical Syntax Node --------------------------------------------------------

SyntaxNodeLogical::
SyntaxNodeLogical()
{

    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_LOGICAL;
    this->left = nullptr;
    this->right = nullptr;
    this->operation_type = OperationType::OPERATION_TYPE_NULL;

}

SyntaxNodeLogical::
~SyntaxNodeLogical()
{
    
}

void SyntaxNodeLogical::
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

