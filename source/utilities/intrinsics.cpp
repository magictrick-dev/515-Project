#include <utilities/intrinsics.hpp>

void 
intrinsic_memory_copy_simple(void* destination, void* source, uint64_t size)
{

    ENSURE_PTR(destination);
    ENSURE_PTR(source);

    for (u64 idx = 0; idx < size; ++idx)
        ((u8*)destination)[idx] = ((u8*)source)[idx];

}

void 
intrinsic_string_copy_simple(char* destination, const char *source, uint64_t size)
{

    ENSURE_PTR(destination);
    ENSURE_PTR(source);

    u64 idx = 0;
    while (source[idx] != '\0')
    {

        if (idx >= (size - 1)) break;
        destination[idx] = source[idx];
        idx++;

    }

    destination[idx] = '\0';


}

u64
intrinsic_string_length(ccptr string)
{

    u64 length = 0;
    while (string[length] != '\0')
        length++;

    return length;


}

