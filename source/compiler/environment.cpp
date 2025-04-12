#include <compiler/environment.hpp>

Environment::
Environment()
{

    // String intern table.
    this->string_table = {};
    this->string_buffer_size    = 1024*4;
    this->string_buffer_pointer = malloc(this->string_buffer_size);
    this->string_buffer_offset  = 0;

    // Symbol table.
    this->symbol_table = {};
    this->symbol_buffer_size    = 1024*4;
    this->symbol_buffer_pointer = malloc(this->symbol_buffer_size);
    this->symbol_buffer_offset  = 0;

    
}

Environment::
~Environment()
{

    if (this->symbol_buffer_pointer != NULL)
    {

        free(this->symbol_buffer_pointer);
        this->symbol_buffer_pointer = NULL;

    }

    if (this->string_buffer_pointer != NULL)
    {

        free(this->string_buffer_pointer);
        this->string_buffer_pointer = NULL;

    }
    
}

vptr Environment::
string_insert(ccptr string)
{

    if (this->string_table.find(string) == this->string_table.end())
    {
        u64 string_length = strlen(string) + 1;
    }

    return (vptr)string;

}

bool Environment::
symbol_exists(ccptr name)
{

    bool exists = this->symbol_table.find(name) != this->symbol_table.end();
    return exists;

}

vptr Environment::
symbol_insert(ccptr name)
{

    char* offset = (char*)this->symbol_buffer_pointer + this->symbol_buffer_offset;
    this->symbol_buffer_offset += ALIGNMENT(sizeof(int32_t), 8);

    this->symbol_table[name] = (vptr)offset;
    return (vptr)offset;

}

vptr Environment::
symbol_search(ccptr name)
{

    vptr result = nullptr;
    if (this->symbol_table.find(name) != this->symbol_table.end())
    {

        result = this->symbol_table[name];

    }

    return result;

}
