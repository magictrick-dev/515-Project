#ifndef SOURCE_DEFINITIONS_HPP
#define SOURCE_DEFINITIONS_HPP
#include <cstdlib>
#include <cstdint>
#include <cassert>

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
typedef float           r32;
typedef double          r64;

#define NOREACH(msg)            assert(!msg)
#define NOIMPL(msg)             assert(!msg)
#define ENSURE(expr)            assert(expr)
#define ENSURE_PTR(ptr)         assert(ptr != NULL)
#define ENSURE_SIZE(size)       assert(size > 0)

#endif
