#ifndef SOURCE_PLATFORM_FILESYSTEM_HPP
#define SOURCE_PLATFORM_FILESYSTEM_HPP
#include <definitions.hpp>
#include <utilities/buffer.hpp>

bool filesystem_path_is_valid(ccptr path);
bool filesystem_path_exists(ccptr path);
bool filesystem_file_exists(ccptr file_name);
bool filesystem_get_file_size(ccptr file_name, u64 *file_size);
bool filesystem_read_entire_file(ccptr file_name, memory_buffer *buffer);
bool filesystem_write_entire_file(ccptr file_name, memory_buffer *buffer);

#endif
