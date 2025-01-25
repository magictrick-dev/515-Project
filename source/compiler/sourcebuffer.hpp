#ifndef SOURCE_COMPILER_SOURCE_BUFFER_HPP
#define SOURCE_COMPILER_SOURCE_BUFFER_HPP
#include <definitions.hpp>
#include <buffer.hpp>
#include <platform/filesystem.hpp>
#include <platform/system.hpp>

// --- Source Buffer -----------------------------------------------------------
//
// Represents the input source file that the user provides. In order for any file
// to be loaded, you must manually invoke the load() method so that the buffer is
// populated with the contents of the file.
//
// Once the file is loaded, you can use the get and advance methods to pull individual
// characters off the buffer. We use a simple file pointer to dictate which byte we are
// at in the buffer, with unget effectively rewinding the file pointer back one.
//
// A consequence of this behavior means that manually tracking the line and column
// position becomes non-trivial since we can't unwind the file pointer and know exactly
// which column it was in when we read the character.
//
//      1.  We could use a stack for the column and line counts. Effective.
//      2.  We track only the line count and use a method to manually calculate the
//          the current column. Also effective, but somewhat computationally costly
//          when we need to query the column.
//
// Option 2 makes sense since we only query the column when we have an error to output.
// How often do errors occur? Not very often. This way we prevent unnecessary STL dependencies
// for edge-case behavior that only requires a small amount of spinning over an array.
//

class SourceBuffer
{

    public:
        inline              SourceBuffer();
        inline              SourceBuffer(const SourceBuffer& other);
        inline              SourceBuffer(SourceBuffer&& other);
        inline virtual     ~SourceBuffer();

        inline SourceBuffer&    operator=(const SourceBuffer& other);
        inline SourceBuffer&    operator=(SourceBuffer&& other);

        inline bool         load(ccptr file_name);
        inline void         unload();
        inline bool         is_loaded() const;
        inline bool         is_eof() const;
        inline bool         is_sof() const;
        inline bool         is_eol() const;

        inline bool         peek(char *c) const;
        inline bool         get(char *c);
        inline bool         unget(char *c);
        inline bool         advance();
        inline void         reset();

        inline u64          get_cursor() const;
        inline i32          get_current_line() const;
        inline i32          get_current_column() const;

        inline string       get_line_string() const;
        inline string       get_line_string_at(i32 cursor) const;
        inline string       get_line_string_from(i32 line, i32 column) const;

    protected:
        buffer  file_buffer;
        u64     file_size;
        i32     file_pointer;
        i32     current_line;
        i32     current_line_position;

};

inline SourceBuffer::
SourceBuffer()
{

    this->file_buffer.data = nullptr;
    this->file_buffer.size = 0;
    this->file_size = 0;
    this->file_pointer = 0;
    this->current_line = 1;
    this->current_line_position = 0;

}

inline SourceBuffer::
SourceBuffer(const SourceBuffer& other)
{

    buffer copy = system_virtual_allocate(NULL, other.file_buffer.size);

    this->file_buffer = copy;
    this->file_size = other.file_size;
    this->file_pointer = other.file_pointer;
    this->current_line = other.current_line;
    this->current_line_position = other.current_line_position;

}

inline SourceBuffer::
SourceBuffer(SourceBuffer&& other)
{

    this->file_buffer = other.file_buffer;
    this->file_pointer = other.file_pointer;
    this->file_size = other.file_size;
    this->current_line = other.current_line;
    this->current_line_position = other.current_line_position;

    other.file_buffer.data = nullptr;
    other.file_buffer.size = 0;
    other.file_pointer = 0;
    other.current_line = 1;
    other.current_line_position = 0;

}

inline SourceBuffer::
~SourceBuffer()
{

    this->unload();

}

inline SourceBuffer& SourceBuffer::
operator=(const SourceBuffer& other)
{

    if (this != &other)
    {

        buffer copy = system_virtual_allocate(NULL, other.file_buffer.size);

        this->file_buffer = copy;
        this->file_pointer = other.file_pointer;
        this->file_size = other.file_size;
        this->current_line = other.current_line;
        this->current_line_position = other.current_line_position;

    }

    return *this;

}

inline SourceBuffer& SourceBuffer::
operator=(SourceBuffer&& other)
{

    if (this != &other)
    {

        this->file_buffer = other.file_buffer;
        this->file_pointer = other.file_pointer;
        this->file_size = other.file_size;
        this->current_line = other.current_line;
        this->current_line_position = other.current_line_position;

        other.file_buffer.data = nullptr;
        other.file_buffer.size = 0;
        other.file_pointer = 0;
        other.current_line = 1;
        other.current_line_position = 0;

    }

    return *this;

}

inline bool SourceBuffer::
load(ccptr file_name)
{

    if (this->file_buffer.data)
        system_virtual_free(file_buffer.data, file_buffer.size);

    // Get the file size.
    u64 file_size = 0;
    if (!filesystem_get_file_size(file_name, &file_size))
        return false;

    // Virtually allocate the buffer.
    buffer buff = system_virtual_allocate(NULL, file_size);

    // Read the file into the buffer.
    if (!filesystem_read_entire_file(file_name, &buff))
        return false;

    // Set the buffer.
    this->file_buffer = buff;

    // Reset the file pointer.
    this->file_size = file_size;
    this->file_pointer = 0;
    this->current_line = 1;
    this->current_line_position = 0;

    return true;

}

inline void SourceBuffer::
unload()
{

    if (file_buffer.data)
        system_virtual_free(file_buffer.data, file_buffer.size);

    this->file_buffer.data = nullptr;
    this->file_buffer.size = 0;
    this->file_size = 0;
    this->file_pointer = 0;
    this->current_line = 1;
    this->current_line_position = 0;

}

inline bool SourceBuffer::
is_loaded() const
{

    return this->file_buffer.data != nullptr;

}

inline bool SourceBuffer::
is_eof() const
{

    return this->file_pointer >= this->file_size;

}

inline bool SourceBuffer::
is_sof() const
{

    return this->file_pointer == 0;

}

inline bool SourceBuffer::
is_eol() const
{

    if (this->is_eof())
        return false;

    if (((char*)this->file_buffer.data)[this->file_pointer] == '\n')
        return true;

    return false;

}

inline bool SourceBuffer::
peek(char *c) const
{

    ENSURE_PTR(c);
    ENSURE_PTR(this->file_buffer.data);

    if (this->is_eof())
        return false;

    *c = ((char*)this->file_buffer.data)[this->file_pointer];
    return true;

}

inline bool SourceBuffer::
get(char *c)
{

    ENSURE_PTR(c);
    ENSURE_PTR(this->file_buffer.data);

    if (this->is_eof())
        return false;

    *c = ((char*)this->file_buffer.data)[this->file_pointer++];

    if (*c == '\n')
    {
        this->current_line++;
        this->current_line_position = this->file_pointer;
    }

    return true;

}

inline bool SourceBuffer::
unget(char *c)
{

    ENSURE_PTR(c);
    ENSURE_PTR(this->file_buffer.data);

    if (this->file_pointer == 0)
        return false;

    *c = ((char*)this->file_buffer.data)[this->file_pointer];
    this->file_pointer--;

    if (*c == '\n')
    {
        this->current_line--;
        this->current_line_position = this->file_pointer;
    }

    if (this->file_pointer == 0)
    {
        this->current_line = 1;
        this->current_line_position = 0;
    }

    return true;

}

inline bool SourceBuffer::
advance()
{

    if (this->is_eof())
        return false;

    char c;
    if (!this->get(&c))
        return false;

    return true;

}

inline void SourceBuffer::
reset()
{

    this->file_pointer = 0;
    this->current_line = 1;
    this->current_line_position = 0;

}

inline i32 SourceBuffer::
get_current_line() const
{

    return this->current_line;

}

inline i32 SourceBuffer::
get_current_column() const
{

    i32 column = this->file_pointer - this->current_line_position + 1;
    return column;

}

inline u64 SourceBuffer::
get_cursor() const
{

    return this->file_pointer;

}

inline string SourceBuffer::
get_line_string() const
{

    string result = this->get_line_string_at(this->current_line_position);
    return result;

}

inline string SourceBuffer::
get_line_string_at(i32 cursor) const
{

    ENSURE(cursor >= 0);
    ENSURE(cursor < this->file_size);

    string result;

    i32 start = cursor;

    while (start > 0)
    {
        if (((char*)this->file_buffer.data)[start] == '\n')
        {
            start++;
            break;
        }

        start--;
    }

    i32 end = cursor;

    while (end < this->file_size)
    {
        if (((char*)this->file_buffer.data)[end] == '\n')
            break;

        end++;
    }


    for (i32 i = start; i < end; i++)
        result += ((char*)this->file_buffer.data)[i];

    return result;

}

inline string SourceBuffer::
get_line_string_from(i32 line, i32 column) const
{

    ENSURE(line > 0);
    ENSURE(column > 0);

    // Calculates the cursor.
    i32 lines = 0;
    i32 offset = 0;
    
    for (offset = 0; offset < this->file_size; ++offset)
    {

        if (((char*)this->file_buffer.data)[offset] == '\n')
            lines++;

        if (lines == line)
            break;

    }

    i32 start = offset + column - 1;
    string result = get_line_string_at(start);
    return result;


}

#endif
