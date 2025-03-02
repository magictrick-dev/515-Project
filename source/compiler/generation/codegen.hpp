#ifndef SOURCE_COMPILER_GENERATION_CODEGEN_HPP
#define SOURCE_COMPILER_GENERATION_CODEGEN_HPP
#include <compiler/parser/visitor.hpp>
#include <utilities/buffer.hpp>
#include <vector>

class CodeGenerator : public SyntaxNodeVisitor
{

    public:
                        CodeGenerator();
        virtual        ~CodeGenerator();
        
        virtual void    visit(SyntaxNodeRoot *node) override;
        virtual void    visit(SyntaxNodeBody *node) override;
        
        virtual void    visit(SyntaxNodeExpressionStatement *node) override;
        
        virtual void    visit(SyntaxNodeExpression *node) override;
        virtual void    visit(SyntaxNodeTerm *node) override;
        virtual void    visit(SyntaxNodeFactor *node) override;
        virtual void    visit(SyntaxNodeMagnitude *node) override;
        virtual void    visit(SyntaxNodeUnary *node) override;
        virtual void    visit(SyntaxNodePrimary *node) override;
        virtual void    visit(SyntaxNodeGrouping *node) override;
        
    protected:
        memory_buffer   buffer;

};

#endif