#include <compiler/compiler.hpp>
#include <compiler/parser/parser.hpp>

Compiler::
Compiler()
    : parser(&this->environment, &this->graph)
{

}

Compiler::
~Compiler()
{

}

bool Compiler::
compile(string source)
{

    if (!this->parser.parse(source))
        return false;

    return true;

}