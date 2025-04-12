#include <compiler/environment.hpp>

Environment::
Environment()
{

    // String intern table.
    this->string_buffer_size    = 1024*1024;
    this->string_buffer_pointer = malloc(this->string_buffer_size);
    this->string_buffer_offset  = 0;

    // Symbol table.
    this->symbol_buffer_size    = 1024*1024;
    this->symbol_buffer_pointer = malloc(this->symbol_buffer_size);
    this->symbol_buffer_offset  = 0;

    
}

Environment::
~Environment()
{
  
}

vptr Environment::
string_insert(string string)
{

    if (this->string_table.find(string) == this->string_table.end())
    {

        cptr result = (char*)this->string_buffer_pointer + this->string_buffer_offset;
        u64 string_length = strlen(string.c_str()) + 1;
        for (u64 i = 0; i < string_length; ++i)
        {

            result[i] = string[i];

        }

        this->string_buffer_offset += string_length;
        this->string_table[string] = result;

    }

    return this->string_table[string];

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
