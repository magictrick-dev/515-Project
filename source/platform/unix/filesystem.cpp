#include <platform/filesystem.hpp>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

bool 
filesystem_get_file_size(ccptr file_name, u64 *file_size)
{

    // If the file exists, get the file size
    struct stat file_stat;
    if (stat(file_name, &file_stat) == 0)
    {
        *file_size = file_stat.st_size;
        return true;
    }


    return false;

}

bool 
filesystem_read_entire_file(ccptr file_name, memory_buffer *buff)
{

    ENSURE_PTR(buff);
    ENSURE_SIZE(buff->size);

    // Open the file.
    int file = open(file_name, O_RDONLY);
    if (file == -1)
    {
        return false;
    }

    // Get the file's size.
    u64 file_size = 0;
    if (!filesystem_get_file_size(file_name, &file_size))
    {
        close(file);
        return false;
    }

    ENSURE(file_size <= buff->size);

    // Read the file.
    u64 bytes_read = read(file, buff->data, file_size);
    if (bytes_read != file_size)
    {
        close(file);
        return false;
    }

    // Close the file.
    close(file);
    return true;

}

bool
filesystem_write_entire_file(ccptr file_name, memory_buffer *buff)
{

    ENSURE_PTR(buff);
    ENSURE_SIZE(buff->size);

    // Open the file.
    int file = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (file == -1)
    {
        return false;
    }

    // Write the file.
    u64 bytes_written = write(file, buff->data, buff->size);
    if (bytes_written != buff->size)
    {
        close(file);
        return false;
    }

    // Close the file.
    close(file);
    return true;

}
