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

    if (codepoint <= 0b01111111)
    {
        output[0] = codepoint;
        output[1] = '\0';
    } 
    else if (codepoint <= 0b01111111111)
    {
        output[0] = 0b11000000 | (codepoint >> 6);
        output[1] = 0b10000000 | (codepoint & 0b00111111);
        output[2] = '\0';
    } 
    else if (codepoint <= 0b1111111111111111) 
    {
        output[0] = 0b11100000 | (codepoint >> 12);
        output[1] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        output[2] = 0b10000000 | (codepoint & 0b00111111);
        output[3] = '\0';
    } 
    else if (codepoint <= 0b100001111111111111111) 
    {
        output[0] = 0b11110000 | (codepoint >> 18);
        output[1] = 0b10000000 | ((codepoint >> 12) & 0b00111111);
        output[2] = 0b10000000 | ((codepoint >> 6) & 0b00111111);
        output[3] = 0b10000000 | (codepoint & 0b00111111);
        output[4] = '\0';
    } 
    else 
    {
        // Invalid code point.
        output[0] = '\0';
    }

}