#include <cassert>
#include <cstdlib>
#include <utilities/allocators.hpp>

static uint64_t total_allocated = 0;
static uint64_t total_released = 0;
static uint64_t current_allocated = 0;
static uint64_t peak_allocated = 0;

typedef struct memory_header
{
    uint64_t user_offset;
    uint64_t user_size;
    uint64_t real_size;
    uint64_t magic;
} memory_header;

void* 
memory_allocate(uint64_t size)
{

    uint64_t required_size = size + sizeof(memory_header);
    memory_header *header = (memory_header*)malloc(required_size);

    if (header == nullptr)
    {
        return nullptr;
    }

    header->user_offset = (uint64_t)header + sizeof(memory_header);
    header->user_size = size;
    header->real_size = required_size;
    header->magic = 0xDEADBEEF;

    total_allocated += size;
    current_allocated += size;

    if (current_allocated > peak_allocated)
    {
        peak_allocated = current_allocated;
    }

    return (void*)header->user_offset;

}

void
memory_release(void *ptr)
{

    assert(ptr != nullptr);

    memory_header *header = (memory_header*)((uint64_t)ptr - sizeof(memory_header));
    assert(header->magic == 0xDEADBEEF);

    total_released += header->user_size;
    current_allocated -= header->user_size;

    free(header);

}

uint64_t
memory_inspect(void *ptr)
{

    assert(ptr != nullptr);

    memory_header *header = (memory_header*)((uint64_t)ptr - sizeof(memory_header));
    assert(header->magic == 0xDEADBEEF);

    return header->user_size;

}

bool
memory_statistics(memory_stats *stats)
{

    assert(stats != nullptr);

    stats->total_allocated = total_allocated;
    stats->total_released = total_released;
    stats->current_allocated = current_allocated;
    stats->peak_allocated = peak_allocated;

    return (stats->total_allocated == stats->total_released);

}

