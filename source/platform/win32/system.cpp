#include <platform/system.hpp>
#include <windows.h>

buffer 
system_virtual_allocate(vptr offset, u64 size)
{
    
    // Round to the nearest page size.
    u64 page_size = system_virtual_page_size();
    u64 required_size = size + (page_size - (size % page_size));
    
    LPVOID result = VirtualAlloc(offset, required_size, MEM_COMMIT|MEM_RESERVER, PAGE_READWRITE);
    ENSURE(result != nullptr);

    buffer buff = {0};
    buff.result = result;
    buff.size = required_size;

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
        PathRemoveFileSpecA(buffer);
        initialized = true;
    }

    return buffer;

}