#ifndef SOURCE_COMPILER_PARSER_NODE_HPP
#define SOURCE_COMPILER_PARSER_NODE_HPP
#include <definitions.hpp>

enum class SyntaxNodeType
{
    SYNTAX_NODE_TYPE_NULL,

    SYNTAX_NODE_TYPE_ROOT,
    SYNTAX_NODE_TYPE_BODY,

    SYNTAX_NODE_TYPE_EXPRESSION_STATEMENT,

    SYNTAX_NODE_TYPE_EXPRESSION,
    SYNTAX_NODE_TYPE_TERM,
    SYNTAX_NODE_TYPE_FACTOR,
    SYNTAX_NODE_TYPE_MAGNITUDE,
    SYNTAX_NODE_TYPE_UNARY,
    SYNTAX_NODE_TYPE_PRIMARY,
    SYNTAX_NODE_TYPE_GROUPING,

};

enum class OperationType
{
    OPERATION_TYPE_NULL,

    OPERATION_TYPE_ADDITION,
    OPERATION_TYPE_SUBTRACTION,
    OPERATION_TYPE_MULTIPLICATION,
    OPERATION_TYPE_DIVISION,
    OPERATION_TYPE_MODULUS,
    OPERATION_TYPE_EXPONENT,
    OPERATION_TYPE_NEGATION,
};

enum class PrimaryType
{
    PRIMARY_TYPE_NULL,

    PRIMARY_TYPE_INTEGER,
    PRIMARY_TYPE_FLOAT,
    PRIMARY_TYPE_STRING,
    PRIMARY_TYPE_IDENTIFIER,
};

class SyntaxNodeVisitor;
struct SyntaxNode
{

    public:
                    SyntaxNode();
        virtual    ~SyntaxNode();
        
        SyntaxNodeType      get_node_type() const;
        virtual void        accept(SyntaxNodeVisitor *visitor) = 0;

    protected:
        SyntaxNodeType node_type;

};

#endif