#ifndef SOURCE_COMPILER_PARSER_NODE_HPP
#define SOURCE_COMPILER_PARSER_NODE_HPP
#include <definitions.hpp>
#include <utilities/string.hpp>

enum class SyntaxNodeType
{
    SYNTAX_NODE_TYPE_NULL,

    SYNTAX_NODE_TYPE_ROOT,
    SYNTAX_NODE_TYPE_BODY,

    SYNTAX_NODE_TYPE_VARIABLE_STATEMENT,
    SYNTAX_NODE_TYPE_READ_STATEMENT,
    SYNTAX_NODE_TYPE_PRINT_STATEMENT,
    SYNTAX_NODE_TYPE_ASSIGNMENT_STATEMENT,
    SYNTAX_NODE_TYPE_EXPRESSION_STATEMENT,

    SYNTAX_NODE_TYPE_EXPRESSION,

    SYNTAX_NODE_TYPE_LOGICAL,
    SYNTAX_NODE_TYPE_RELATIONAL,
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
    OPERATION_TYPE_POSITIVE,
    OPERATION_TYPE_LOGICAL_AND,
    OPERATION_TYPE_LOGICAL_OR,
    OPERATION_TYPE_LOGICAL_NOT,
    OPERATION_TYPE_LESS,
    OPERATION_TYPE_LESS_EQUAL,
    OPERATION_TYPE_GREATER,
    OPERATION_TYPE_GREATER_EQUAL,
    OPERATION_TYPE_EQUAL,
    OPERATION_TYPE_NOT_EQUAL,
};

enum class PrimaryType
{

    PRIMARY_TYPE_NULL,

    PRIMARY_TYPE_INTEGER,
    PRIMARY_TYPE_FLOAT,
    PRIMARY_TYPE_STRING,
    PRIMARY_TYPE_IDENTIFIER,
    PRIMARY_TYPE_BOOLEAN,

};

enum class EvaluationType
{
    
    EVALUATION_TYPE_NONE,

    EVALUATION_TYPE_NULL,
    EVALUATION_TYPE_INT4,
    EVALUATION_TYPE_STRING_LITERAL,
    EVALUATION_TYPE_FLOAT,
    EVALUATION_TYPE_BOOLEAN,

};

inline string evaluation_type_to_string(EvaluationType type)
{

    switch (type)
    {

        case EvaluationType::EVALUATION_TYPE_NONE:              return "NONE";
        case EvaluationType::EVALUATION_TYPE_NULL:              return "NULL";
        case EvaluationType::EVALUATION_TYPE_INT4:              return "INT4";
        case EvaluationType::EVALUATION_TYPE_STRING_LITERAL:    return "STRING LITERAL";
        case EvaluationType::EVALUATION_TYPE_FLOAT:             return "FLOAT";
        case EvaluationType::EVALUATION_TYPE_BOOLEAN:           return "BOOLEAN";
        default:
        {
            ENSURE(!"Whoops, reached unimplemented / uncaught type!");
        };

    }

    return "";

}

class SyntaxNodeVisitor;
struct SyntaxNode
{

    public:
                    SyntaxNode();
        virtual    ~SyntaxNode();
        
        SyntaxNodeType      get_node_type() const;
        virtual void        accept(SyntaxNodeVisitor *visitor) = 0;

    public:
        EvaluationType evaluation_type = EvaluationType::EVALUATION_TYPE_NONE;

    protected:
        SyntaxNodeType node_type;

};

#endif
