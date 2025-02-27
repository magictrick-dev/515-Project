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
        shared_ptr<SyntaxNodeRoot>      match_root();

    protected:
        shared_ptr<SyntaxNode> base_node;
        std::vector<shared_ptr<SyntaxNode>> nodes;

        Tokenizer       tokenizer;
        Graph          *graph;
        Environment    *environment;

};

#endif