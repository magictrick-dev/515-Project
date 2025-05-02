#ifndef SOURCE_COMPILER_REFERENCE_HPP
#define SOURCE_COMPILER_REFERENCE_HPP
#include <compiler/parser/visitor.hpp>

class ASTReferenceOutput : public SyntaxNodeVisitor
{
    
    public:
                        ASTReferenceOutput(int tab_size = 4);
        virtual        ~ASTReferenceOutput();
        
        virtual void    visit(SyntaxNodeRoot *node) override;
        virtual void    visit(SyntaxNodeBody *node) override;
        
        virtual void    visit(SyntaxNodeVariableStatement *node) override;
        virtual void    visit(SyntaxNodeReadStatement *node) override;
        virtual void    visit(SyntaxNodePrintStatement *node) override;
        virtual void    visit(SyntaxNodeAssignmentStatement *node) override;
        virtual void    visit(SyntaxNodeExpressionStatement *node) override;
        
        virtual void    visit(SyntaxNodeExpression *node) override;
        virtual void    visit(SyntaxNodeLogical *node) override;
        virtual void    visit(SyntaxNodeRelational *node) override;
        virtual void    visit(SyntaxNodeTerm *node) override;
        virtual void    visit(SyntaxNodeFactor *node) override;
        virtual void    visit(SyntaxNodeMagnitude *node) override;
        virtual void    visit(SyntaxNodeUnary *node) override;
        virtual void    visit(SyntaxNodePrimary *node) override;
        
    protected:
        void        push_tabs();
        void        pop_tabs();
        void        print_tabs() const;
        
    protected:
        int         tabs;
        int         tabs_size;

};

#endif
