#include <windows.h>
#include <platform/filesystem.hpp>

bool
filesystem_path_is_valid(ccptr path)
{

    // Is it a file or directory?   
    DWORD attrib = GetFileAttributesA(path);
    if (attrib == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }

    return true;

}

bool
filesystem_path_exists(ccptr path)
{
    
    DWORD attrib = GetFileAttributesA(path);
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));

}

bool
filesystem_file_exists(ccptr file_name)
{
    DWORD attrib = GetFileAttributesA(file_name);
    return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool
filesystem_get_file_size(ccptr file_name, u64 *file_size)
{

    ENSURE_PTR(file_size);

    HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ,
            0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(file, &size))
    {
        CloseHandle(file);
        return false;
    }

    *file_size = size.QuadPart;
    CloseHandle(file);
    return true;

}

bool
filesystem_read_entire_file(ccptr file_name, buffer *buff)
{

    ENSURE_PTR(buff);
    ENSURE_PTR(buff->data);

    HANDLE file = CreateFileA(file_name, GENERIC_READ, FILE_SHARE_READ,
            0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    u64 file_size;
    if (!filesystem_get_file_size(file_name, &file_size))
    {
        CloseHandle(file);
        return false;
    }

    ENSURE(file_size <= buff->size);

    DWORD bytes_read;
    if (!ReadFile(file, buff->data, (DWORD)file_size, &bytes_read, 0))
    {
        CloseHandle(file);
        return false;
    }

    CloseHandle(file);
    return true;

}

bool
filesystem_write_entire_file(ccptr file_name, buffer *buff)
{

    ENSURE_PTR(buff);
    ENSURE_PTR(buff->data);

    HANDLE file = CreateFileA(file_name, GENERIC_WRITE, 0,
            0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (file == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    DWORD bytes_written;
    if (!WriteFile(file, buff->data, (DWORD)buff->size, &bytes_written, 0))
    {
        CloseHandle(file);
        return false;
    }

    CloseHandle(file);
    return true;

}
