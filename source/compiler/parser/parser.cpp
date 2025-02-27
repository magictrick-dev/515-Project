#include <compiler/parser/parser.hpp>
#include <compiler/errorhandler.hpp>

SyntaxParser::
SyntaxParser(Environment *environment, Graph *graph)
    : graph(graph), environment(environment)
{
    
}

SyntaxParser::
~SyntaxParser()
{
    
}

bool SyntaxParser::
parse(string source)
{
    
    try
    {

        if (!this->tokenizer.initialize(source.c_str()))
            throw CompilerResourceException(__LINE__, source, "Failed to initialize the tokenizer, filepath is invalid.");
        
        if (CompilerException::get_exceptions_called() != 0)
            return false;
        
        this->base_node = this->match_root();
        return true;

    }
    catch (CompilerException &exception)
    {

        std::cout << exception.what() << std::endl;
        return false;

    }

    return true;
    
}

// --- Matching Functions ------------------------------------------------------

shared_ptr<SyntaxNodeRoot> SyntaxParser::
match_root()
{
    
    
    
    return nullptr;

}