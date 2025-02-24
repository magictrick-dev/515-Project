#include <compiler/parser/parser.hpp>

SyntaxParser::
SyntaxParser()
{
    
}

SyntaxParser::
~SyntaxParser()
{
    
}

bool SyntaxParser::
parse(string source)
{
    
    if (!this->tokenizer.initialize(source.c_str()))
        return false;
    
    return true;
    
}