// -----------------------------------------------------------------------------
//
// Developed by Chris DeJong, January - May 2025. 
// CSCI-490-D1 / CSCI-515 Principles of Compilers
//
// --- Language Grammar --------------------------------------------------------
// 
// The following language grammar describes the AST structure of the language.
// 
//      root            : body
//      body            : (statement)*
//      statement       : expression
//      expression      : term ("+" | "-" term)*
//      term            : factor ("*" | "/" | "mod" factor)*
//      factor          : magnitude ("^" magnitude)*
//      magnitude       : unary
//      unary           : ("+" | "-") unary | primary
//      primary         : number | string | identifier | "(" expression ")"
//
// -----------------------------------------------------------------------------

#ifndef SOURCE_COMPILER_PARSER_PARSER_HPP
#define SOURCE_COMPILER_PARSER_PARSER_HPP
#include <definitions.hpp>
#include <vector>
#include <utilities/string.hpp>
#include <compiler/lexer/tokenizer.hpp>
#include <compiler/environment.hpp>
#include <compiler/graph.hpp>
#include <compiler/parser/subnodes.hpp>

class SyntaxParser
{

    public:
                        SyntaxParser(Environment *environment, Graph *graph);
        virtual        ~SyntaxParser();

        bool            parse(string source);
        
    protected:
        template <typename T, typename ...Args> shared_ptr<T> create_node(Args... args);

    protected:
        shared_ptr<SyntaxNode>      match_root();
        shared_ptr<SyntaxNode>      match_body();
        shared_ptr<SyntaxNode>      match_statements();
        
        shared_ptr<SyntaxNode>      match_expression_statement();
        
        shared_ptr<SyntaxNode>      match_expression();
        shared_ptr<SyntaxNode>      match_term();
        shared_ptr<SyntaxNode>      match_factor();
        shared_ptr<SyntaxNode>      match_magnitude();
        shared_ptr<SyntaxNode>      match_unary();
        shared_ptr<SyntaxNode>      match_primary();

    protected:
        string source_name;
        shared_ptr<SyntaxNode> base_node;
        std::vector<shared_ptr<SyntaxNode>> nodes;

        Tokenizer       tokenizer;
        Graph          *graph;
        Environment    *environment;

};

#endif