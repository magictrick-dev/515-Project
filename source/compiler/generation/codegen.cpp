#include <compiler/generation/codegen.hpp>
#include <compiler/generation/intrinsics.hpp>
#include <platform/system.hpp>

// --- Code Generation ---------------------------------------------------------
//
// Underlying code generation routine.
//

CodeGenerator::
CodeGenerator(u64 buffer_size, Graph *graph, Environment *environment)
{
    
    buffer = system_virtual_allocate(NULL, buffer_size, true);
    this->graph = graph;
    this->environment = environment;
    
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
    memory_buffer_write_u8(&this->buffer, 0xC3);

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
visit(SyntaxNodePrintStatement *node)
{

    for (auto expression : node->expressions)
    {

        // Now generate.
        switch (expression->evaluation_type)
        {

            case EvaluationType::EVALUATION_TYPE_INT4:
            {

                // Loads everything into EAX.
                expression->accept(this);

                memory_buffer_write_u8(&this->buffer, 0x89); // MOV EDI, EAX
                memory_buffer_write_u8(&this->buffer, 0xC7);

                int64_t print_cast = (int64_t)print_int; // Dirty cast the pointer.

                memory_buffer_write_u8(&this->buffer, 0x48); // MOV ESI, [print_int]
                memory_buffer_write_u8(&this->buffer, 0xBE);
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, 0xFF);
                memory_buffer_write_u8(&this->buffer, 0xD6);

            } break;

            case EvaluationType::EVALUATION_TYPE_STRING_LITERAL:
            {

                expression->accept(this);

#if defined(_WIN32)
                // MOV RCX, RAX
                // Windows ABI ????????????????
                // ????????????????????????????
                memory_buffer_write_u8(&this->buffer, 0x48);
                memory_buffer_write_u8(&this->buffer, 0x89);
                memory_buffer_write_u8(&this->buffer, 0xC1);  // C1 = ModR/M for RCX, RAX
#else
                memory_buffer_write_u8(&this->buffer, 0x48); // MOV RDI, RAX 
                memory_buffer_write_u8(&this->buffer, 0x89);
                memory_buffer_write_u8(&this->buffer, 0xC7);
#endif

                long int print_cast = (long int)print_string; // Dirty cast the pointer.

                memory_buffer_write_u8(&this->buffer, 0x48); // MOV ESI, [print_int]
                memory_buffer_write_u8(&this->buffer, 0xBE);
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, 0xFF);
                memory_buffer_write_u8(&this->buffer, 0xD6);

            } break;

            default:
            {

                ENSURE(!"Unimplemented evaluation type condition.");
                
            }

        }

    }

    return;

}

void CodeGenerator::
visit(SyntaxNodeExpressionStatement *node)
{
    
    memory_buffer_reset_position(&this->buffer);

    node->expression->accept(this);

    /*
    int result = this->operator()();
    std::cout << "Executing" << std::endl;
    std::cout << "    Code Size:  " << this->buffer.position << " bytes" << std::endl;
    std::cout << "    Result:     " << result << std::endl;
    */

}

void CodeGenerator::
visit(SyntaxNodeExpression *node)
{

    node->expression->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeTerm *node)
{
    
    
    switch (node->operation_type)
    {
        case OperationType::OPERATION_TYPE_ADDITION:
        {

            // Loads the left side into EAX.
            node->left->accept(this);
            
            // Push left from EAX.
            memory_buffer_write_u8(&this->buffer,  0x50);
            
            // This loads the right side into EAX.
            node->right->accept(this);
            
            // Pop into ECX.
            memory_buffer_write_u8(&this->buffer,  0x59);
            
            // ADD EAX, ECX
            memory_buffer_write_u8(&this->buffer,  0x01);
            memory_buffer_write_u8(&this->buffer,  0xC8);

        } break;
        
        case OperationType::OPERATION_TYPE_SUBTRACTION:
        {

            // Loads the right side into EAX.
            node->right->accept(this);

            // Push right from EAX.
            memory_buffer_write_u8(&this->buffer,  0x50);

            // This loads the left side into EAX.
            node->left->accept(this);

            // Pop into ECX.
            memory_buffer_write_u8(&this->buffer,  0x59);

            // SUB EAX, ECX
            memory_buffer_write_u8(&this->buffer,  0x29);
            memory_buffer_write_u8(&this->buffer,  0xC8);

        } break;
        
        default:
        {
            
            NOREACH("Invalid or unimplemented operation type.");

        }

    }
    
}

void CodeGenerator::
visit(SyntaxNodeFactor *node)
{
    
    switch (node->operation_type)
    {

        case OperationType::OPERATION_TYPE_MULTIPLICATION:
        {
            
            // Loads the left side into EAX.
            node->left->accept(this);
            
            // Push left from EAX.
            memory_buffer_write_u8(&this->buffer,  0x50);
            
            // This loads the right side into EAX.
            node->right->accept(this);
            
            // Pop into ECX.
            memory_buffer_write_u8(&this->buffer,  0x59);

            // IMUL EDX:EAX, ECX
            memory_buffer_write_u8(&this->buffer,  0xF7);
            memory_buffer_write_u8(&this->buffer,  0xE9);

        } break;
        
        case OperationType::OPERATION_TYPE_DIVISION:
        {
            
            // Loads the right side into EAX.
            node->right->accept(this);

            // Push right from EAX.
            memory_buffer_write_u8(&this->buffer,  0x50);

            // This loads the left side into EAX.
            node->left->accept(this);

            // Pop into ECX.
            memory_buffer_write_u8(&this->buffer,  0x59);
            
            // Sign extend EAX into EDX:EAX.
            memory_buffer_write_u8(&this->buffer,  0x99);

            // IDIV EDX:EAX, ECX
            memory_buffer_write_u8(&this->buffer,  0xF7);
            memory_buffer_write_u8(&this->buffer,  0xF9);

        } break;
        
        case OperationType::OPERATION_TYPE_MODULUS:
        {
            
            // Loads the right side into EAX.
            node->right->accept(this);

            // Push right from EAX.
            memory_buffer_write_u8(&this->buffer,  0x50);

            // This loads the left side into EAX.
            node->left->accept(this);

            // Pop into ECX.
            memory_buffer_write_u8(&this->buffer,  0x59);
            
            // Sign extend EAX into EDX.
            memory_buffer_write_u8(&this->buffer,  0x99);

            // IDIV EDX:EAX, ECX
            memory_buffer_write_u8(&this->buffer,  0xF7);
            memory_buffer_write_u8(&this->buffer,  0xF9);
            
            // XCHG EAX, EDX
            memory_buffer_write_u8(&this->buffer,  0x92);

        } break;

    }

}

void CodeGenerator::
visit(SyntaxNodeMagnitude *node)
{
    
    switch (node->operation_type)
    {

        case OperationType::OPERATION_TYPE_EXPONENT:
        {
            
            // Preloads EAX with the LHS.
            node->left->accept(this);

            // Push EAX.
            memory_buffer_write_u8(&this->buffer,   0x50);

            // Preloads EAX with the RHS.
            node->right->accept(this);

            // Now we do the magic.
            memory_buffer_write_u8(&this->buffer,   0x41); // MOV R10, EAX
            memory_buffer_write_u8(&this->buffer,   0x89);
            memory_buffer_write_u8(&this->buffer,   0xC2);
            memory_buffer_write_u8(&this->buffer,   0x41); // POP R9
            memory_buffer_write_u8(&this->buffer,   0x8F);
            memory_buffer_write_u8(&this->buffer,   0xC1);
            memory_buffer_write_u8(&this->buffer,   0x45); // XOR R8, R8
            memory_buffer_write_u8(&this->buffer,   0x31);
            memory_buffer_write_u8(&this->buffer,   0xc0);
            memory_buffer_write_u8(&this->buffer,   0x45); // TEST R10, R10
            memory_buffer_write_u8(&this->buffer,   0x85);
            memory_buffer_write_u8(&this->buffer,   0xd2);
            memory_buffer_write_u8(&this->buffer,   0x7c); // JL 1EH
            memory_buffer_write_u8(&this->buffer,   0x1e);
            memory_buffer_write_u8(&this->buffer,   0x41); // INC R8
            memory_buffer_write_u8(&this->buffer,   0xff);
            memory_buffer_write_u8(&this->buffer,   0xc0);
            memory_buffer_write_u8(&this->buffer,   0x45); // TEST R10, R10
            memory_buffer_write_u8(&this->buffer,   0x85);
            memory_buffer_write_u8(&this->buffer,   0xd2);
            memory_buffer_write_u8(&this->buffer,   0x74); // JE 16H
            memory_buffer_write_u8(&this->buffer,   0x16);
            memory_buffer_write_u8(&this->buffer,   0x41); // TEST R10, 00000001H
            memory_buffer_write_u8(&this->buffer,   0xf7);
            memory_buffer_write_u8(&this->buffer,   0xc2);
            memory_buffer_write_u8(&this->buffer,   0x01);
            memory_buffer_write_u8(&this->buffer,   0x00);
            memory_buffer_write_u8(&this->buffer,   0x00);
            memory_buffer_write_u8(&this->buffer,   0x00);
            memory_buffer_write_u8(&this->buffer,   0x74); // JE 04H
            memory_buffer_write_u8(&this->buffer,   0x04);
            memory_buffer_write_u8(&this->buffer,   0x45); // IMUL R8, R9
            memory_buffer_write_u8(&this->buffer,   0x0f);
            memory_buffer_write_u8(&this->buffer,   0xaf);
            memory_buffer_write_u8(&this->buffer,   0xc1);
            memory_buffer_write_u8(&this->buffer,   0x45); // IMUL R9, R9
            memory_buffer_write_u8(&this->buffer,   0x0f);
            memory_buffer_write_u8(&this->buffer,   0xaf);
            memory_buffer_write_u8(&this->buffer,   0xc9);
            memory_buffer_write_u8(&this->buffer,   0x41); // SAR R10, 1
            memory_buffer_write_u8(&this->buffer,   0xd1);
            memory_buffer_write_u8(&this->buffer,   0xfa);
            memory_buffer_write_u8(&this->buffer,   0xeb); // JMP  -27H
            memory_buffer_write_u8(&this->buffer,   0xe5);
            memory_buffer_write_u8(&this->buffer,   0x44); // MOV EAX, R8
            memory_buffer_write_u8(&this->buffer,   0x89);
            memory_buffer_write_u8(&this->buffer,   0xc0);

        } break;

    }

}

void CodeGenerator::
visit(SyntaxNodeUnary *node)
{
    
    switch (node->operation_type)
    {
        
        case OperationType::OPERATION_TYPE_POSITIVE:
        {
            
            node->right->accept(this);
            
        } break;
        
        case OperationType::OPERATION_TYPE_NEGATION:
        {
            
            node->right->accept(this);
            
            // NEG EAX
            memory_buffer_write_u8(&this->buffer,  0xF7);
            memory_buffer_write_u8(&this->buffer,  0xD8);
            
        } break;
        
        default:
        {
            
            NOREACH("Invalid or unimplemented operation type.");
            
        }
        
    }

}

void CodeGenerator::
visit(SyntaxNodePrimary *node)
{
    

    switch (node->primary_type)
    {

        case PrimaryType::PRIMARY_TYPE_INTEGER:
        {
            
            i32 value = std::stoi(node->value);

            // MOV EAX, value
            memory_buffer_write_u8(&this->buffer,   0xB8);
            
            // IMM32
            memory_buffer_write_u8(&this->buffer,   (value >>  0) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >>  8) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 16) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 24) & 0xFF);
            
        } break;

        case PrimaryType::PRIMARY_TYPE_STRING:
        {
    

            string type_string = node->value;
            type_string.erase(0,1);
            type_string.pop_back();
            cptr string_result = (cptr)this->environment->string_insert(type_string.c_str());

            i64 value = (i64)string_result;

            // MOVE RAX, value
            memory_buffer_write_u8(&this->buffer,   0x48);
            memory_buffer_write_u8(&this->buffer,   0xB8);

            // IMM64
            memory_buffer_write_u8(&this->buffer,   (value >>  0) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >>  8) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 16) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 24) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 32) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 40) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 48) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 56) & 0xFF);

        } break;
        
        default:       
        {
            
            NOREACH("Unimplemented condition, most likely.");

        } break;

    }


}

