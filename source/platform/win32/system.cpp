#include <platform/system.hpp>
#include <windows.h>

memory_buffer 
system_virtual_allocate(vptr offset, u64 size, bool executable)
{
    
    // Round to the nearest page size.
    u64 page_size = system_virtual_page_size();
    u64 required_size = size + (page_size - (size % page_size));
    
    DWORD protection = PAGE_READWRITE;
    if (executable)
        protection = PAGE_EXECUTE_READWRITE;
    
    LPVOID result = VirtualAlloc(offset, required_size, MEM_COMMIT|MEM_RESERVE, protection);
    ENSURE(result != nullptr);

    memory_buffer buff = {0};
    buff.data   = result;
    buff.size   = required_size;

    return buff;

}

void
system_virtual_free(vptr ptr, u64 size)
{

    VirtualFree(ptr, 0, MEM_RELEASE);

}

u64
system_virtual_page_size()
{

    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    return system_info.dwPageSize;

}

ccptr
system_get_current_working_directory()
{

    static char buffer[MAX_PATH];
    static bool initialized = false;
    if (!initialized)
    {
        GetCurrentDirectoryA(MAX_PATH, buffer);
        initialized = true;
    }
    return buffer;

}

ccptr
system_get_executable_directory()
{

    static char buffer[MAX_PATH];
    static bool initialized = false;
    if (!initialized)
    {

        GetModuleFileNameA(nullptr, buffer, MAX_PATH);

        // Remove the executable name.
        char* last_slash = strrchr(buffer, '\\');
        if (last_slash)
        {
            *last_slash = '\0';
        }

        initialized = true;

    }

    return buffer;

}
