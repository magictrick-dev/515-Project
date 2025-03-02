#ifndef SOURCE_COMPILER_COMPILER_HPP
#define SOURCE_COMPILER_COMPILER_HPP
#include <definitions.hpp>
#include <compiler/environment.hpp>
#include <compiler/graph.hpp>
#include <utilities/string.hpp>
#include <compiler/parser/parser.hpp>

class Compiler
{

    public:
                    Compiler();
        virtual    ~Compiler();
        
        bool        compile(string source, bool dump_reference);

    protected:
        SyntaxParser    parser;
        Environment     environment;
        Graph           graph;

};

#endif