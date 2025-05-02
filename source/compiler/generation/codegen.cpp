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

    std::cout << "CODE GENERATION SIZE " << this->buffer.position << std::endl;
    std::cout << "VARIABLE STORAGE TABLE SIZE: " << this->environment->symbol_table_commit() << std::endl;
    std::cout << "STRING INTERN TABLE SIZE: " << this->environment->string_table_commit() << std::endl;

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
visit(SyntaxNodeVariableStatement *node)
{

    i64 address = (i64)node->address;

    // XOR EAX, EAX
    memory_buffer_write_u8(&this->buffer, 0x31);
    memory_buffer_write_u8(&this->buffer, 0xC0);

    // MOV RDI, imm64
    memory_buffer_write_u8(&this->buffer, 0x48);
    memory_buffer_write_u8(&this->buffer, 0xBF);
    memory_buffer_write_u8(&this->buffer, (address >>  0) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >>  8) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 16) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 24) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 32) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 40) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 48) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 56) & 0xFF);

    // MOV [RDI], RAX
    memory_buffer_write_u8(&this->buffer, 0x48);
    memory_buffer_write_u8(&this->buffer, 0x89);
    memory_buffer_write_u8(&this->buffer, 0x07);

}

void CodeGenerator::
visit(SyntaxNodeReadStatement *node)
{


    int64_t cast = (int64_t)read_int; // Dirty cast the pointer.

    memory_buffer_write_u8(&this->buffer, 0x48); // MOV RSI, [print_int]
    memory_buffer_write_u8(&this->buffer, 0xBE);
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;
    memory_buffer_write_u8(&this->buffer, cast & 0xff); cast >>= 8;

    memory_buffer_write_u8(&this->buffer, 0xFF); // CALL RSI
    memory_buffer_write_u8(&this->buffer, 0xD6);

    // Results should be in EAX.
    int64_t address = (int64_t)this->environment->symbol_search(node->variable_name.c_str());
    ENSURE(address != 0);

    memory_buffer_write_u8(&this->buffer, 0x48); // MOV RDI, IMM ADDRESS
    memory_buffer_write_u8(&this->buffer, 0xBF);
    memory_buffer_write_u8(&this->buffer, (address >>  0) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >>  8) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 16) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 24) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 32) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 40) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 48) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 56) & 0xFF);

    memory_buffer_write_u8(&this->buffer, 0x89); // MOV [RDI], EAX
    memory_buffer_write_u8(&this->buffer, 0x07);

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

                memory_buffer_write_u8(&this->buffer, 0x48); // MOV RSI, [print_int]
                memory_buffer_write_u8(&this->buffer, 0xBE);
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;

                memory_buffer_write_u8(&this->buffer, 0xFF); // CALL RSI
                memory_buffer_write_u8(&this->buffer, 0xD6);

            } break;

            case EvaluationType::EVALUATION_TYPE_BOOLEAN:
            {

                // Loads everything into EAX.
                expression->accept(this);

                memory_buffer_write_u8(&this->buffer, 0x89); // MOV EDI, EAX
                memory_buffer_write_u8(&this->buffer, 0xC7);

                int64_t print_cast = (int64_t)print_bool; // Dirty cast the pointer.

                memory_buffer_write_u8(&this->buffer, 0x48); // MOV RSI, [print_int]
                memory_buffer_write_u8(&this->buffer, 0xBE);
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;
                memory_buffer_write_u8(&this->buffer, print_cast & 0xff); print_cast >>= 8;

                memory_buffer_write_u8(&this->buffer, 0xFF); // CALL RSI
                memory_buffer_write_u8(&this->buffer, 0xD6);

            } break;

            case EvaluationType::EVALUATION_TYPE_STRING_LITERAL:
            {

                expression->accept(this);

                memory_buffer_write_u8(&this->buffer, 0x48); // MOV RDI, RAX 
                memory_buffer_write_u8(&this->buffer, 0x89);
                memory_buffer_write_u8(&this->buffer, 0xC7);

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
                
                memory_buffer_write_u8(&this->buffer, 0xFF); // CALL RSI
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
visit(SyntaxNodeAssignmentStatement *node)
{
    
    // This will automatically load the expression result into EAX.
    node->expression->accept(this);

    // Get the address of the variable.
    int64_t address = (int64_t)this->environment->symbol_search(node->variable_name.c_str());
    ENSURE(address != 0);

    // Perform the move.
    memory_buffer_write_u8(&this->buffer, 0x48); // MOV RDI, IMM ADDRESS
    memory_buffer_write_u8(&this->buffer, 0xBF);
    memory_buffer_write_u8(&this->buffer, (address >>  0) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >>  8) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 16) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 24) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 32) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 40) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 48) & 0xFF);
    memory_buffer_write_u8(&this->buffer, (address >> 56) & 0xFF);

    memory_buffer_write_u8(&this->buffer, 0x89); // MOV [RDI], EAX
    memory_buffer_write_u8(&this->buffer, 0x07);



}

void CodeGenerator::
visit(SyntaxNodeExpressionStatement *node)
{
    
    node->expression->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeExpression *node)
{

    node->expression->accept(this);

}

void CodeGenerator::
visit(SyntaxNodeLogical *node)
{

    switch (node->operation_type)
    {

        case OperationType::OPERATION_TYPE_LOGICAL_OR:
        {

            // Places result into AL (not EAX).
            node->left->accept(this);

            // TST AL, 0x01.
            memory_buffer_write_u8(&this->buffer, 0xA8);
            memory_buffer_write_u8(&this->buffer, 0x01);

                                                        
            // JNZ (32-bit) over right.
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x85);

            u64 jump_offset = this->buffer.position;
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);

            u64 current_offset = this->buffer.position; // Store current offset.

            // Places result into AL.
            node->right->accept(this);

            u64 ending_offset = this->buffer.position; // Ending offset.
            i32 value = (ending_offset - current_offset);
            this->buffer.position = jump_offset;
            memory_buffer_write_u8(&this->buffer,   (value >>  0) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >>  8) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 16) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 24) & 0xFF);

            this->buffer.position = ending_offset; // Restore our position.


        } break;

        case OperationType::OPERATION_TYPE_LOGICAL_AND:
        {
            // Places result into AL (not EAX).
            node->left->accept(this);

            // TST AL, 0x01.
            memory_buffer_write_u8(&this->buffer, 0xA8);
            memory_buffer_write_u8(&this->buffer, 0x01);

                                                        
            // JNZ (32-bit) over right.
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x84);

            u64 jump_offset = this->buffer.position;
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);
            memory_buffer_write_u8(&this->buffer, 0x00);

            u64 current_offset = this->buffer.position; // Store current offset.

            // Places result into AL.
            node->right->accept(this);

            u64 ending_offset = this->buffer.position; // Ending offset.
            i32 value = (ending_offset - current_offset);
            this->buffer.position = jump_offset;
            memory_buffer_write_u8(&this->buffer,   (value >>  0) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >>  8) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 16) & 0xFF);
            memory_buffer_write_u8(&this->buffer,   (value >> 24) & 0xFF);

            this->buffer.position = ending_offset; // Restore our position.

        } break;

        case OperationType::OPERATION_TYPE_LOGICAL_NOT:
        {

            // Since a logical not only has a right hand operand...
            node->right->accept(this);

            // XOR al, 0x01;
            memory_buffer_write_u8(&this->buffer, 0x34);
            memory_buffer_write_u8(&this->buffer, 0x01);

        } break;

    }

}

void CodeGenerator::
visit(SyntaxNodeRelational *node)
{

    // Loads the left side into EAX.
    node->left->accept(this);
    
    // Push left from EAX.
    memory_buffer_write_u8(&this->buffer,  0x50);
    
    // This loads the right side into EAX.
    node->right->accept(this);
    
    // Pop into ECX.
    memory_buffer_write_u8(&this->buffer,  0x59);

    // CMP ECX, EAX
    memory_buffer_write_u8(&this->buffer, 0x39);
    memory_buffer_write_u8(&this->buffer, 0xC1);

    switch (node->operation_type)
    {

        case OperationType::OPERATION_TYPE_LESS:
        {

            // SETL AL (less than)
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x9C);
            memory_buffer_write_u8(&this->buffer, 0xC0);
            

        } break;

        case OperationType::OPERATION_TYPE_LESS_EQUAL:
        {

            // SETLE AL
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x9E);
            memory_buffer_write_u8(&this->buffer, 0xC0);

        } break;

        case OperationType::OPERATION_TYPE_GREATER:
        {

            // SETG AL
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x9F);
            memory_buffer_write_u8(&this->buffer, 0xC0);

        } break;

        case OperationType::OPERATION_TYPE_GREATER_EQUAL:
        {

            // SETGE AL
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x9D);
            memory_buffer_write_u8(&this->buffer, 0xC0);

        } break;

        case OperationType::OPERATION_TYPE_EQUAL:
        {

            // SETE AL
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x94);
            memory_buffer_write_u8(&this->buffer, 0xC0);

        } break;

        case OperationType::OPERATION_TYPE_NOT_EQUAL:
        {

            // SETNE AL
            memory_buffer_write_u8(&this->buffer, 0x0F);
            memory_buffer_write_u8(&this->buffer, 0x95);
            memory_buffer_write_u8(&this->buffer, 0xC0);

        } break;


    }

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

        case PrimaryType::PRIMARY_TYPE_BOOLEAN:
        {

            string boolean_value = node->value;
            i64 value = 0;

            if (boolean_value == "true") value = 1;

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

        case PrimaryType::PRIMARY_TYPE_IDENTIFIER:
        {

            string identifier = node->value;
            i64 address = (i64)this->environment->symbol_search(identifier.c_str());
            ENSURE(address != 0);

            // MOV RDI, IMM
            memory_buffer_write_u8(&this->buffer, 0x48);
            memory_buffer_write_u8(&this->buffer, 0xBF);
            memory_buffer_write_u8(&this->buffer, (address >>  0) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >>  8) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 16) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 24) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 32) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 40) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 48) & 0xFF);
            memory_buffer_write_u8(&this->buffer, (address >> 56) & 0xFF);

            // MOV RAX, [RDI]
            memory_buffer_write_u8(&this->buffer, 0x48);
            memory_buffer_write_u8(&this->buffer, 0x8B);
            memory_buffer_write_u8(&this->buffer, 0x07);

        } break;
        
        default:       
        {
            
            NOREACH("Unimplemented condition, most likely.");

        } break;

    }


}

