#ifndef SOURCE_COMPILER_PARSER_SUBNODES_HPP
#define SOURCE_COMPILER_PARSER_SUBNODES_HPP
#include <compiler/parser/node.hpp>
#include <utilities/string.hpp>

// --- Root Syntax Node --------------------------------------------------------
// 
// Represents the base node returned from the AST. The root node itself will typically
// be a container for the first node in the AST to be traversed.
//
// -----------------------------------------------------------------------------

class SyntaxNodeRoot : public SyntaxNode
{

    public:
                        SyntaxNodeRoot();
        virtual        ~SyntaxNodeRoot();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> base_node;

};

// --- Expression Syntax Node --------------------------------------------------
// 
// The expression syntax node contains a complete logical expression. Generally,
// expression nodes are either single primitive nodes, or a combination of logical
// and/or mathematical operations.
//
// -----------------------------------------------------------------------------

class SyntaxNodeExpression : public SyntaxNode
{

    public:
                        SyntaxNodeExpression();
        virtual        ~SyntaxNodeExpression();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> expression;

};

// --- Term Syntax Node --------------------------------------------------------
// 
// Denotes a term in a mathematical expression.
//
// -----------------------------------------------------------------------------

class SyntaxNodeTerm : public SyntaxNode
{

    public:
                        SyntaxNodeTerm();
        virtual        ~SyntaxNodeTerm();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode>  left;
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

};

// --- Factor Syntax Node ------------------------------------------------------
// 
// Represents a factor in a mathematical expression.
//
// -----------------------------------------------------------------------------

class SyntaxNodeFactor : public SyntaxNode
{

    public:
                        SyntaxNodeFactor();
        virtual        ~SyntaxNodeFactor();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode>  left;
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

};

// --- Magnitude Syntax Node ---------------------------------------------------
// 
// Represents a magnitude in a mathematical expression.
// 
// -----------------------------------------------------------------------------

class SyntaxNodeMagnitude : public SyntaxNode
{

    public:
                        SyntaxNodeMagnitude();
        virtual        ~SyntaxNodeMagnitude();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;

    public:
        shared_ptr<SyntaxNode>  left;
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

};

// --- Unary Syntax Node -------------------------------------------------------
// 
// Represents a unary operation in a mathematical expression.
// 
// -----------------------------------------------------------------------------

class SyntaxNodeUnary : public SyntaxNode
{

    public:
                        SyntaxNodeUnary();
        virtual        ~SyntaxNodeUnary();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;

    public:
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

};

// --- Primary Syntax Node -----------------------------------------------------
// 
// Represents either a literal value or a sub-expression grouping.
//
// -----------------------------------------------------------------------------

class SyntaxNodePrimary : public SyntaxNode
{

    public:
                        SyntaxNodePrimary();
        virtual        ~SyntaxNodePrimary();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        PrimaryType primary_type;
        string value;

};

// --- Grouping Syntax Node ----------------------------------------------------
// 
// Represents a grouping of expressions.
// 
// -----------------------------------------------------------------------------

class SyntaxNodeGrouping : public SyntaxNode
{

    public:
                        SyntaxNodeGrouping();
        virtual        ~SyntaxNodeGrouping();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> expression;

};

#endif