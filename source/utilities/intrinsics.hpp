#ifndef SOURCE_UTILITIES_INTRINSICS_HPP
#define SOURCE_UTILITIES_INTRINSICS_HPP
#include <definitions.hpp>

void    intrinsic_memory_copy_simple(vptr destination, vptr source, u64 size);
void    intrinsic_string_copy_simple(cptr destination, ccptr source, u64 size);
u64     intrinsic_string_length(ccptr string);
bool    intrinsic_string_compare(cptr left, ccptr right);
void    intrinsic_codepoint_to_utf8(u32 codepoint, cptr output);

#endif
