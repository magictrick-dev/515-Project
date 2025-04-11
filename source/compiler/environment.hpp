#ifndef SOURCE_COMPILER_ENVIRONMENT_HPP
#define SOURCE_COMPILER_ENVIRONMENT_HPP
#include <definitions.hpp>
#include <utilities/string.hpp>
#include <unordered_map>

class Environment
{

    public:
                    Environment();
        virtual    ~Environment();

        vptr        string_insert(ccptr string);

        bool        symbol_exists(ccptr name);
        vptr        symbol_insert(ccptr name);
        vptr        symbol_search(ccptr name);

    protected:
        std::unordered_map<string, vptr> string_table;
        vptr        string_buffer_pointer;
        u64         string_buffer_size;
        u64         string_buffer_offset;

        std::unordered_map<string, vptr> symbol_table;
        vptr        symbol_buffer_pointer;
        u64         symbol_buffer_size;
        u64         symbol_buffer_offset;
        
};

#endif
