#ifndef SOURCE_PLATFORM_FILESYSTEM_HPP
#define SOURCE_PLATFORM_FILESYSTEM_HPP
#include <definitions.hpp>
#include <buffer.hpp>

bool filesystem_get_file_size(ccptr file_name, u64 *file_size);
bool filesystem_read_entire_file(ccptr file_name, buffer *buff);
bool filesystem_write_entire_file(ccptr file_name, buffer *buff);

#endif
