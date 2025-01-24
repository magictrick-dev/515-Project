#ifndef SOURCE_DEFINITIONS_HPP
#define SOURCE_DEFINITIONS_HPP
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <string>

typedef uint8_t         u8;
typedef uint16_t        u16;
typedef uint32_t        u32;
typedef uint64_t        u64;
typedef int8_t          i8;
typedef int16_t         i16;
typedef int32_t         i32;
typedef int64_t         i64;
typedef void*           vptr;
typedef const char*     ccptr;
typedef char*           cptr;

#define MEMORY_ALLOCATE(u64) malloc(u64)
#define MEMORY_RELEASE(ptr) free(ptr)

#define ENSURE(expr) assert(expr)
#define ENSURE_PTR(ptr) assert(ptr != NULL)
#define ENSURE_SIZE(size) assert(size > 0)

using std::string; // We may want to write our own to track memory allocations.

#endif
