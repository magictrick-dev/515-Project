#ifndef SOURCE_COMPILER_PARSER_PARSER_HPP
#define SOURCE_COMPILER_PARSER_PARSER_HPP
#include <definitions.hpp>
#include <compiler/lexer/tokenizer.hpp>
#include <compiler/environment.hpp>
#include <compiler/graph.hpp>
#include <utilities/string.hpp>

class SyntaxParser
{

    public:
                    SyntaxParser();
        virtual    ~SyntaxParser();

        bool        parse(string source);

    protected:
        Tokenizer       tokenizer;
        Graph          *graph;
        Environment    *environment;

};

#endif
