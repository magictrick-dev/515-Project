#ifndef SOURCE_COMPILER_COMPILER_HPP
#define SOURCE_COMPILER_COMPILER_HPP
#include <definitions.hpp>
#include <compiler/environment.hpp>
#include <compiler/graph.hpp>
#include <utilities/string.hpp>

class Compiler
{

    public:
                    Compiler();
        virtual    ~Compiler();

        bool        parse(string source);
        bool        validate();
        bool        generate();

    protected:
        Environment environment;
        Graph graph;

};

#endif
