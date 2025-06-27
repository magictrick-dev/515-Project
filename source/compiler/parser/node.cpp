#include <compiler/parser/node.hpp>

SyntaxNode::
SyntaxNode()
{
    this->node_type = SyntaxNodeType::SYNTAX_NODE_TYPE_NULL;
}

SyntaxNode::
~SyntaxNode()
{
    
}

SyntaxNodeType SyntaxNode::
get_node_type() const
{
    return this->node_type;
}
