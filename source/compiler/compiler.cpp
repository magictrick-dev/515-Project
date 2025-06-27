#include <compiler/compiler.hpp>
#include <compiler/parser/parser.hpp>
#include <compiler/reference.hpp>
#include <compiler/generation/codegen.hpp>

Compiler::
Compiler() : parser(&this->environment, &this->graph)
{

    this->environment = {};
    this->graph = {};

}

Compiler::
~Compiler()
{

}

bool Compiler::
compile(string source, bool no_reference, bool no_generate, bool no_execute)
{

    bool result = this->parser.parse(source);
    
    if (!no_reference)
    {
        
        std::cout << "-- Displaying tree output." << std::endl;
        ASTReferenceOutput output;
        this->parser.accept(&output);

    }
    else
    {
        std::cout << "-- Reference output is disabled." << std::endl;
    }
    
    if (!no_generate)
    {
        CodeGenerator generator(4*1024*1024, &this->graph, &this->environment);
        this->parser.accept(&generator);
        if (!no_execute) generator();
        else
        {
            std::cout << "-- Code execution is disabled." << std::endl;

        }
    }
    else
    {
        std::cout << "-- Code generation is disabled." << std::endl;
    }

    std::cout << "\n-- Runtime complete. (Newline is automatically added.)" << std::endl;

    return result;

}
