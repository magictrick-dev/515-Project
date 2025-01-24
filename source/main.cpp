#include <iostream>
#include <cstdint>
#include <unistd.h>
#include <sys/stat.h>

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

u64
unix_get_file_size(ccptr file_name)
{
    struct stat file_stat;
    if (stat(file_name, &file_stat) == 0)
    {
        return file_stat.st_size;
    }
    return 0;
}

u64
unix_page_size()
{

    return sysconf(_SC_PAGESIZE);

}

bool
unix_virtual_allocate(buffer* buffer, u64 size)
{

}

typedef struct buffer
{
    vptr data;
    u64 size;
} buffer;

int
main(int argc, char ** argv)
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    ccptr file_name = argv[1];
    u64 file_size = unix_get_file_size(file_name);



    return 0;
}


