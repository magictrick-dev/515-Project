#ifndef SOURCE_COMPILER_GENERATION_CODEGEN_HPP
#define SOURCE_COMPILER_GENERATION_CODEGEN_HPP
#include <compiler/parser/visitor.hpp>
#include <compiler/environment.hpp>
#include <compiler/graph.hpp>
#include <utilities/buffer.hpp>
#include <vector>

typedef int (*program_executable_fptr)(void);

class CodeGenerator : public SyntaxNodeVisitor
{

    public:
                        CodeGenerator(u64 buffer_size, Graph *graph, Environment *environment);
        virtual        ~CodeGenerator();
        
        int             operator()(); // functor fun time
        
        virtual void    visit(SyntaxNodeRoot *node) override;
        virtual void    visit(SyntaxNodeBody *node) override;
        
        virtual void    visit(SyntaxNodeVariableStatement *node) override;
        virtual void    visit(SyntaxNodeReadStatement *node) override;
        virtual void    visit(SyntaxNodePrintStatement *node) override;
        virtual void    visit(SyntaxNodeAssignmentStatement *node) override;
        virtual void    visit(SyntaxNodeExpressionStatement *node) override;
        virtual void    visit(SyntaxNodeConditionalStatement *node) override;
        virtual void    visit(SyntaxNodeWhileStatement *node) override;
        
        virtual void    visit(SyntaxNodeExpression *node) override;
        virtual void    visit(SyntaxNodeLogical *node) override;
        virtual void    visit(SyntaxNodeRelational *node) override;
        virtual void    visit(SyntaxNodeTerm *node) override;
        virtual void    visit(SyntaxNodeFactor *node) override;
        virtual void    visit(SyntaxNodeMagnitude *node) override;
        virtual void    visit(SyntaxNodeUnary *node) override;
        virtual void    visit(SyntaxNodePrimary *node) override;
        
    protected:
        memory_buffer   buffer;
        Environment    *environment;
        Graph          *graph;

};

#endif
