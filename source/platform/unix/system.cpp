#include <cstdio>
#include <platform/system.hpp>
#include <unistd.h>
#include <sys/mman.h>

memory_buffer      
system_virtual_allocate(vptr offset, u64 size)
{

    // In case the user does something silly.
    if (size == 0)
    {
        buffer result = {0};
        return result;
    }

    // Round the request size up to the page size.
    u64 actual_size = size + (system_virtual_page_size() - (size % system_virtual_page_size()));

    // Allocate the memory.
    vptr ptr = mmap(offset, actual_size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    // Ensure the memory was allocated.
    ENSURE_PTR(ptr);

    // Return the buffer.
    memory_buffer result = {0};
    result.data = ptr;
    result.size = actual_size;

    return result;

}

void        
system_virtual_free(vptr ptr, u64 size)
{

    // Unmap the memory.
    munmap(ptr, size);

}

u64         
system_virtual_page_size()
{

    return sysconf(_SC_PAGESIZE);

}

ccptr       
system_get_current_working_directory()
{

    ccptr result = getcwd(NULL, 0);
    return result;

}

ccptr       
system_get_executable_directory()
{

    static char buffer[1024] = {0};
    static bool initialized = false;

    if (initialized)
    {
        return buffer;
    }

    // Get the executable path.
    ccptr path = "/proc/self/exe";

    // Open the file.
    FILE* file = fopen(path, "r");
    ENSURE_PTR(file);

    // Get the path.
    bool res = readlink(path, buffer, sizeof(buffer));
    ENSURE(res);

    // Close the file.
    fclose(file);

    // Return the path.
    return buffer;

}

