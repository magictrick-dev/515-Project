#include <compiler/generation/codegen.hpp>
#include <platform/system.hpp>

CodeGenerator::
CodeGenerator(u64 buffer_size)
{
    
    buffer = system_virtual_allocate(NULL, buffer_size, true);
    
}

CodeGenerator::
~CodeGenerator()
{
    
    system_virtual_free(buffer.data, buffer.size);
    
}

int CodeGenerator::
operator()()
{
    
    program_executable_fptr program = (program_executable_fptr)buffer.data;  
    return program();
    
}

// --- Visitor Implementation --------------------------------------------------

void CodeGenerator::
visit(SyntaxNodeRoot *node)
{

    node->base_node->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeBody *node)
{

    for (auto &statement : node->statements)
    {
        
        statement->accept(this);
        
    }

}

void CodeGenerator::
visit(SyntaxNodeExpressionStatement *node)
{
    
    memory_buffer_reset_position(&this->buffer);

    node->expression->accept(this);

    // RET
    memory_buffer_write_u8(&this->buffer,   0xC3);

    int result = this->operator()();
    std::cout << result << std::endl;

}

void CodeGenerator::
visit(SyntaxNodeExpression *node)
{

    node->expression->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeTerm *node)
{

}

void CodeGenerator::
visit(SyntaxNodeFactor *node)
{

}

void CodeGenerator::
visit(SyntaxNodeMagnitude *node)
{

}

void CodeGenerator::
visit(SyntaxNodeUnary *node)
{

}

void CodeGenerator::
visit(SyntaxNodePrimary *node)
{
    

    switch (node->primary_type)
    {

        case PrimaryType::PRIMARY_TYPE_INTEGER:
        {
            
            i32 value = std::stoi(node->value);
            std::cout << "Integer: " << value << std::endl;

            // MOV EAX, value
            memory_buffer_write_u8(&this->buffer, 0xB8);
            
            // IMM32
            memory_buffer_write_u32(&this->buffer, value);
            
            // PUSH EAX
            memory_buffer_write_u8(&this->buffer, 0x50);
            
        } break;
        
        default:       
        {
            
            NOREACH("Unimplemented condition, most likely.");

        } break;

    }


}

void CodeGenerator::
visit(SyntaxNodeGrouping *node)
{

}