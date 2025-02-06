#ifndef SOURCE_UTILITIES_ALLOCATORS_HPP
#define SOURCE_UTILITIES_ALLOCATORS_HPP
#include <cstdint>

// --- Memory Allocation Wrappers ----------------------------------------------
//
// The memory allocation wrapper is design to provide the front-end a way of
// determining if the application is properly releasing memory when it is used.
// The memory_statistics() function returns true if the total allocated is the
// same as total freed. This metric may not always be the best way to determine
// if the application is performing memory lifetime parity since there are cases
// where the application intentionally chooses to leak memory and let the OS handle
// the cleanup. Therefore, the structure it fills out contains more detailed info
// on this process.
//
// The allocation functions wrap malloc/free.
//

void*                                       memory_allocate(uint64_t size);
void                                        memory_release(void *ptr);
uint64_t                                    memory_inspect(void *ptr);

template <typename T, typename... Args> T*
memory_new(Args... args)
{

    T *ptr = (T*)memory_allocate(sizeof(T));
    return new (ptr) T(args...);

}

template <typename T> void
memory_delete(T* ptr)
{

    ptr->~T();
    memory_release(ptr);

}

typedef struct memory_stats
{
    uint64_t   total_allocated;
    uint64_t   total_released;
    uint64_t   current_allocated;
    uint64_t   peak_allocated;
} memory_stats;

bool        memory_statistics(memory_stats *stats);

#endif
