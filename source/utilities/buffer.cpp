#include "utilities/intrinsics.hpp"
#include <utilities/buffer.hpp>

void    
memory_buffer_reset_position(memory_buffer* buffer)
{

    ENSURE_PTR(buffer);

    buffer->position = 0;

}

bool    
memory_buffer_shift_position(memory_buffer* buffer, i64 shift)
{

    ENSURE_PTR(buffer);

    if (shift < 0)
    {
        if (buffer->position < (u64)(-shift))
        {
            return false;
        }
        buffer->position += shift;
        return true;
    }
    else
    {
        if (buffer->position + shift > buffer->size)
        {
            return false;
        }
        buffer->position += shift;
        return true;
    }

    return false;
}

bool    
memory_buffer_shift_position_to(memory_buffer* buffer, u64 position)
{

    ENSURE_PTR(buffer);

    ENSURE_PTR(buffer);
    if (position < buffer->size)
    {
        buffer->position = position;
        return true;
    }

    return false;
}

bool    
memory_buffer_is_eob(memory_buffer* buffer)
{

    ENSURE_PTR(buffer);

    if (buffer->position >= buffer->size)
    {
        return true;
    }

    return false;
}

bool    
memory_buffer_is_sob(memory_buffer* buffer)
{

    ENSURE_PTR(buffer);

    if (buffer->position == 0)
    {
        return true;
    }

    return false;
}


bool    
memory_buffer_read_char(memory_buffer* buffer, char *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        *value = ((char*)buffer->data)[buffer->position];
        buffer->position++;
        return true;
    }

    return false;

}

bool    
memory_buffer_read_u8(memory_buffer* buffer, u8 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        *value = ((u8*)buffer->data)[buffer->position];
        buffer->position++;
        return true;
    }

    return false;
}

bool    
memory_buffer_read_u16(memory_buffer* buffer, u16 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u16) <= buffer->size)
    {
        *value = ((u16*)buffer->data)[buffer->position];
        buffer->position += sizeof(u16);
        return true;
    }

    return false;

}

bool    
memory_buffer_read_u32(memory_buffer* buffer, u32 *value)
{
    
    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u32) <= buffer->size)
    {
        *value = ((u32*)buffer->data)[buffer->position];
        buffer->position += sizeof(u32);
        return true;
    }

    return false;
}

bool    
memory_buffer_read_u64(memory_buffer* buffer, u64 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u64) <= buffer->size)
    {
        *value = ((u64*)buffer->data)[buffer->position];
        buffer->position += sizeof(u64);
        return true;
    }

    return false;

}

bool    
memory_buffer_read_i8(memory_buffer* buffer, i8 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        *value = ((i8*)buffer->data)[buffer->position];
        buffer->position++;
        return true;
    }

    return false;

}

bool    
memory_buffer_read_i16(memory_buffer* buffer, i16 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i16) <= buffer->size)
    {
        *value = ((i16*)buffer->data)[buffer->position];
        buffer->position += sizeof(i16);
        return true;
    }

    return false;

}

bool    
memory_buffer_read_i32(memory_buffer* buffer, i32 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i32) <= buffer->size)
    {
        *value = ((i32*)buffer->data)[buffer->position];
        buffer->position += sizeof(i32);
        return true;
    }

    return false;

}

bool    
memory_buffer_read_i64(memory_buffer* buffer, i64 *value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i64) <= buffer->size)
    {
        *value = ((i64*)buffer->data)[buffer->position];
        buffer->position += sizeof(i64);
        return true;
    }

    return false;
}

bool    
memory_buffer_read_bytes(memory_buffer* buffer, vptr value, u64 size)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + size <= buffer->size)
    {
        intrinsic_memory_copy_simple(value, (u8*)buffer->data + buffer->position, size);
        buffer->position += size;
        return true;
    }

    return false;

}


bool    
memory_buffer_write_char(memory_buffer* buffer, char value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        ((char*)buffer->data)[buffer->position] = value;
        buffer->position++;
        return true;
    }

    return false;

}

bool    
memory_buffer_write_u8(memory_buffer* buffer, u8 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        ((u8*)buffer->data)[buffer->position] = value;
        buffer->position++;
        return true;
    }

    return false;

}

bool    
memory_buffer_write_u16(memory_buffer* buffer, u16 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u16) <= buffer->size)
    {
        ((u16*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(u16);
        return true;
    }

    return false;

}

bool    
memory_buffer_write_u32(memory_buffer* buffer, u32 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u32) <= buffer->size)
    {
        ((u32*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(u32);
        return true;
    }

    return false;

}

bool    
memory_buffer_write_u64(memory_buffer* buffer, u64 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(u64) <= buffer->size)
    {
        ((u64*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(u64);
        return true;
    }

    return false;

}

bool    
memory_buffer_write_i8(memory_buffer* buffer, i8 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position < buffer->size)
    {
        ((i8*)buffer->data)[buffer->position] = value;
        buffer->position++;
        return true;
    }

    return false;

}

bool    
memory_buffer_write_i16(memory_buffer* buffer, i16 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i16) <= buffer->size)
    {
        ((i16*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(i16);
        return true;
    }

    return false;

}

bool    
memory_buffer_write_i32(memory_buffer* buffer, i32 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i32) <= buffer->size)
    {
        ((i32*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(i32);
        return true;
    }

    return false;

}

bool    
memory_buffer_write_i64(memory_buffer* buffer, i64 value)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);

    if (buffer->position + sizeof(i64) <= buffer->size)
    {
        ((i64*)buffer->data)[buffer->position] = value;
        buffer->position += sizeof(i64);
        return true;
    }

    return false;
}

bool    
memory_buffer_write_bytes(memory_buffer* buffer, vptr value, u64 size)
{

    ENSURE_PTR(buffer);
    ENSURE_PTR(buffer->data);
    ENSURE_PTR(value);

    if (buffer->position + size <= buffer->size)
    {
        intrinsic_memory_copy_simple((u8*)buffer->data + buffer->position, value, size);
        buffer->position += size;
        return true;
    }

    return false;

}

