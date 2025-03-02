#include <compiler/compiler.hpp>
#include <compiler/parser/parser.hpp>
#include <compiler/reference.hpp>

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
compile(string source, bool dump_reference)
{

    bool result = this->parser.parse(source);
    
    if (dump_reference == true)
    {
        
        std::cout << "-- Displaying tree output." << std::endl;
        ASTReferenceOutput output;
        this->parser.accept(&output);

    }

    return result;

}