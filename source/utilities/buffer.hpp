#ifndef SOURCE_BUFFER_HPP
#define SOURCE_BUFFER_HPP
#include <definitions.hpp>

// --- Memory Buffer -----------------------------------------------------------
//
// A memory buffer points to a region of memory and provides some helpful utilities
// to read and write to the buffer using a position pointer. Many of the methods
// return a boolean to indicate a success or failure for a given operation to ensure
// that the user doesn't attempt to read and write out of bounds.
//
// Generally, the memory buffer alone is sufficient as a standalone buffer. I don't
// typically use the accompanying read/write methods unless I am performing strict
// buffer read/write routines that need them. Most of the time, I will manually
// manipulate the buffer's data directly.
//
// Operations that return false indicate no change occured (probably because the
// operation was out of bounds). Operations that return true indicate that the
// operation was successful and the output is valid.
//
// NOTE(Chris): Specifically for this project, the following code is code I brought
//              over from my game engine.
//

typedef struct memory_buffer
{
    vptr    data;
    u64     size;
    u64     position;
} memory_buffer;

void    memory_buffer_reset_position(memory_buffer* buffer);
bool    memory_buffer_shift_position(memory_buffer* buffer, i64 shift);
bool    memory_buffer_shift_position_to(memory_buffer* buffer, u64 position);
bool    memory_buffer_is_eob(memory_buffer* buffer);
bool    memory_buffer_is_sob(memory_buffer* buffer);

bool    memory_buffer_read_char(memory_buffer* buffer, char *value);
bool    memory_buffer_read_u8(memory_buffer* buffer, u8 *value);
bool    memory_buffer_read_u16(memory_buffer* buffer, u16 *value);
bool    memory_buffer_read_u32(memory_buffer* buffer, u32 *value);
bool    memory_buffer_read_u64(memory_buffer* buffer, u64 *value);
bool    memory_buffer_read_i8(memory_buffer* buffer, i8 *value);
bool    memory_buffer_read_i16(memory_buffer* buffer, i16 *value);
bool    memory_buffer_read_i32(memory_buffer* buffer, i32 *value);
bool    memory_buffer_read_i64(memory_buffer* buffer, i64 *value);
bool    memory_buffer_read_bytes(memory_buffer* buffer, vptr value, u64 size);

bool    memory_buffer_write_char(memory_buffer* buffer, char value);
bool    memory_buffer_write_u8(memory_buffer* buffer, u8 value);
bool    memory_buffer_write_u16(memory_buffer* buffer, u16 value);
bool    memory_buffer_write_u32(memory_buffer* buffer, u32 value);
bool    memory_buffer_write_u64(memory_buffer* buffer, u64 value);
bool    memory_buffer_write_i8(memory_buffer* buffer, i8 value);
bool    memory_buffer_write_i16(memory_buffer* buffer, i16 value);
bool    memory_buffer_write_i32(memory_buffer* buffer, i32 value);
bool    memory_buffer_write_i64(memory_buffer* buffer, i64 value);
bool    memory_buffer_write_bytes(memory_buffer* buffer, vptr value, u64 size);


#endif
