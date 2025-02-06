#ifndef SOURCE_UTILITIES_INTRINSICS_HPP
#define SOURCE_UTILITIES_INTRINSICS_HPP
#include <definitions.hpp>

void    intrinsic_memory_copy_simple(vptr destination, vptr source, u64 size);
void    intrinsic_string_copy_simple(cptr destination, ccptr source, u64 size);
u64     intrinsic_string_length(ccptr string);

#endif
