#ifndef SOURCE_COMPILER_PARSER_VISITOR_HPP
#define SOURCE_COMPILER_PARSER_VISITOR_HPP
#include <compiler/parser/node.hpp>
#include <compiler/parser/subnodes.hpp>

class SyntaxNodeVisitor
{

    public:
        virtual void    visit(SyntaxNodeRoot *node);
        virtual void    visit(SyntaxNodeExpression *node);
        virtual void    visit(SyntaxNodeTerm *node);
        virtual void    visit(SyntaxNodeFactor *node);
        virtual void    visit(SyntaxNodeMagnitude *node);
        virtual void    visit(SyntaxNodeUnary *node);
        virtual void    visit(SyntaxNodePrimary *node);
        virtual void    visit(SyntaxNodeGrouping *node);

    protected:

};

#endif