#ifndef SOURCE_PLATFORM_SYSTEM_HPP
#define SOURCE_PLATFORM_SYSTEM_HPP
#include <definitions.hpp>
#include <buffer.hpp>

memory_buffer   system_virtual_allocate(vptr offset, u64 size);
void            system_virtual_free(vptr ptr, u64 size);
u64             system_virtual_page_size();
ccptr           system_get_current_working_directory();
ccptr           system_get_executable_directory();

#endif
