#ifndef SOURCE_COMPILER_PARSER_SUBNODES_HPP
#define SOURCE_COMPILER_PARSER_SUBNODES_HPP
#include <compiler/parser/node.hpp>
#include <utilities/string.hpp>
#include <vector>

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

// --- Body Syntax Node --------------------------------------------------------
// 
// Represents a body of statements.
//
// -----------------------------------------------------------------------------

class SyntaxNodeBody : public SyntaxNode
{

    public:
                        SyntaxNodeBody();
        virtual        ~SyntaxNodeBody();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        std::vector<shared_ptr<SyntaxNode>> statements;

};

// --- Variable Statement Syntax Node ------------------------------------------
// 
// Represents a variable statement.
//
// -----------------------------------------------------------------------------

class SyntaxNodeVariableStatement : public SyntaxNode
{

    public:
                        SyntaxNodeVariableStatement();
        virtual        ~SyntaxNodeVariableStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        string variable_name;
        vptr address;

};

// --- Expression Statement Syntax Node -----------------------------------------
// 
// Represents a print statement.
//
// -----------------------------------------------------------------------------

class SyntaxNodeReadStatement : public SyntaxNode
{

    public:
                        SyntaxNodeReadStatement();
        virtual        ~SyntaxNodeReadStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        string variable_name; // Where?

};

// --- Expression Statement Syntax Node -----------------------------------------
// 
// Represents a print statement.
//
// -----------------------------------------------------------------------------

class SyntaxNodePrintStatement : public SyntaxNode
{

    public:
                        SyntaxNodePrintStatement();
        virtual        ~SyntaxNodePrintStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        std::vector<shared_ptr<SyntaxNode>> expressions;

};

// --- While Statement Syntax Node ---------------------------------------------
//
// While nodes.
//
// -----------------------------------------------------------------------------

class SyntaxNodeWhileStatement : public SyntaxNode
{
    public:
                        SyntaxNodeWhileStatement();
        virtual        ~SyntaxNodeWhileStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> condition;
        std::vector<shared_ptr<SyntaxNode>> statements;

};

// --- Conditional Statement Syntax Node ---------------------------------------
//
// Conditional nodes (if/else).
//
// -----------------------------------------------------------------------------

class SyntaxNodeConditionalStatement : public SyntaxNode
{

    public:
                        SyntaxNodeConditionalStatement();
        virtual        ~SyntaxNodeConditionalStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> condition;
        std::vector<shared_ptr<SyntaxNode>> conditional_if;
        std::vector<shared_ptr<SyntaxNode>> conditional_else;

};

// --- Assignment Statement Syntax Node ----------------------------------------
// 
// Represents a single expression statement.
//
// -----------------------------------------------------------------------------

class SyntaxNodeAssignmentStatement : public SyntaxNode
{

    public:
                        SyntaxNodeAssignmentStatement();
        virtual        ~SyntaxNodeAssignmentStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        string variable_name;
        shared_ptr<SyntaxNode> expression;

};

// --- Expression Statement Syntax Node -----------------------------------------
// 
// Represents a single expression statement.
//
// -----------------------------------------------------------------------------

class SyntaxNodeExpressionStatement : public SyntaxNode
{

    public:
                        SyntaxNodeExpressionStatement();
        virtual        ~SyntaxNodeExpressionStatement();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode> expression;

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

// --- Logical Syntax Node -----------------------------------------------------
// 
// Logical.
//
// -----------------------------------------------------------------------------

class SyntaxNodeLogical : public SyntaxNode
{

    public:
                        SyntaxNodeLogical();
        virtual        ~SyntaxNodeLogical();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode>  left;
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

};

// --- Relational Syntax Node --------------------------------------------------
// 
// Relational.
//
// -----------------------------------------------------------------------------

class SyntaxNodeRelational : public SyntaxNode
{

    public:
                        SyntaxNodeRelational();
        virtual        ~SyntaxNodeRelational();

        virtual void    accept(SyntaxNodeVisitor *visitor) override;
        
    public:
        shared_ptr<SyntaxNode>  left;
        shared_ptr<SyntaxNode>  right;
        OperationType           operation_type;

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

#endif
