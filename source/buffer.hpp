#ifndef SOURCE_BUFFER_HPP
#define SOURCE_BUFFER_HPP
#include <definitions.hpp>

typedef struct memory_buffer
{
    vptr data;
    u64 size;
} memory_buffer;

#endif
