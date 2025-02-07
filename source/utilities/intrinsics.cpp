#include <utilities/intrinsics.hpp>
#include <iostream>

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

bool
intrinsic_string_compare(cptr left, ccptr right)
{

    u64 idx = 0;
    while (left[idx] != '\0' && right[idx] != '\0')
    {

        if (left[idx] != right[idx])
            return false;

        idx++;

    }

    return left[idx] == right[idx];

}

void
intrinsic_codepoint_to_utf8(i32 codepoint, cptr output)
{

    if (codepoint <= 0x7F) {
        output[0] = codepoint;
        output[1] = '\0';
    } else if (codepoint <= 0x7FF) {
        output[0] = 0xC0 | (codepoint >> 6);
        output[1] = 0x80 | (codepoint & 0x3F);
        output[2] = '\0';
    } else if (codepoint <= 0xFFFF) {
        output[0] = 0xE0 | (codepoint >> 12);
        output[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        output[2] = 0x80 | (codepoint & 0x3F);
        output[3] = '\0';
    } else if (codepoint <= 0x10FFFF) {
        output[0] = 0xF0 | (codepoint >> 18);
        output[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        output[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        output[3] = 0x80 | (codepoint & 0x3F);
        output[4] = '\0';
    } else {
        output[0] = '\0';  // Invalid code point
    }

    std::cout << codepoint << " : " << output << std::endl;
    //printf("%s\n", output);

}