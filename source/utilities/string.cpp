#include <cstdio>
#include <utilities/string.hpp>
#include <utilities/allocators.hpp>
#include <utilities/intrinsics.hpp>

#if 0

// Uncle bob stans be shoketh
static char scratch_buffer[256];

string::
string()
{

    this->resize(8);

}

string::
string(ccptr str)
{

    u64 length = intrinsic_string_length(str) + 1;
    this->resize(length);
    intrinsic_string_copy_simple(this->buffer, str, length);

}

string::
string(const string &str)
{

    u64 length = str.size;
    this->resize(length);
    intrinsic_string_copy_simple(this->buffer, str.buffer, length);

}

string::
string(string &&str)
{

    this->buffer = str.buffer;
    this->size = str.size;
    str.buffer = nullptr;
    str.size = 0;

}

string::
~string()
{

    this->clear();

}

string& string::
operator=(ccptr str)
{

    u64 length = intrinsic_string_length(str) + 1;
    this->resize(length);
    intrinsic_string_copy_simple(this->buffer, str, length);

    return *this;

}

string& string::
operator=(const string &str)
{

    u64 length = str.size;
    this->resize(length);
    intrinsic_string_copy_simple(this->buffer, str.buffer, length);

    return *this;

}

string& string::
operator=(string &&str)
{

    this->clear();
    this->buffer = str.buffer;
    this->size = str.size;
    str.buffer = nullptr;
    str.size = 0;

    return *this;

}

char& string::
operator[](u64 index)
{

    ENSURE(index < this->size);
    return this->buffer[index];

}

char string::
operator[](u64 index) const
{

    ENSURE(index < this->size);
    return this->buffer[index];

}

bool string::
operator==(ccptr str) const
{

    return intrinsic_string_compare(this->buffer, str);

}

bool string::
operator==(const string &str) const
{

    return intrinsic_string_compare(this->buffer, str.buffer);

}

bool string::
operator!=(ccptr str) const
{

    return !intrinsic_string_compare(this->buffer, str);

}

bool string::
operator!=(const string &str) const
{

    return !intrinsic_string_compare(this->buffer, str.buffer);

}

string& string::
operator+=(ccptr str)
{

    u64 length = intrinsic_string_length(str) + 1;
    u64 new_size = this->size + length;
    this->resize(new_size);
    intrinsic_string_copy_simple(this->buffer + intrinsic_string_length(this->buffer), str, length);

    return *this;

}

string& string::
operator+=(const string &str)
{

    u64 length = str.size;
    u64 new_size = this->size + length;
    this->resize(new_size);
    intrinsic_string_copy_simple(this->buffer + intrinsic_string_length(this->buffer), str.buffer, length);

    return *this;

}

string& string::
operator+=(char c)
{

    u64 new_size = this->size + 2;
    this->resize(new_size);
    this->buffer[intrinsic_string_length(this->buffer)] = c;
    this->buffer[intrinsic_string_length(this->buffer)] = '\0';

    return *this;

}

string& string::
operator+=(int i)
{

    snprintf(scratch_buffer, 256, "%d", i);
    *this += scratch_buffer;
    return *this;

}

string& string::
operator+=(float i)
{

    snprintf(scratch_buffer, 256, "%f", i);
    *this += scratch_buffer;
    return *this;

}

string& string::
operator+=(double i)
{

    snprintf(scratch_buffer, 256, "%f", i);
    *this += scratch_buffer;
    return *this;

}

u64 string::
length() const
{

    return intrinsic_string_length(this->buffer);

}

cptr string::
data()
{

    return this->buffer;

}

ccptr string::
c_str() const
{

    return this->buffer;

}

void string::
resize(u64 size)
{

    if (this->size >= size)
        return;

    char *buffer = (char*)memory_allocate(size);
    for (u64 i = 0; i < size; i++)
        buffer[i] = '\0';
    if (this->buffer != nullptr)
        intrinsic_string_copy_simple(buffer, this->buffer, size);

    this->clear();
    this->buffer = buffer;
    this->size = size;

}

void string::
clear()
{

    if (this->buffer != nullptr)
        memory_release(this->buffer);
    this->buffer = nullptr;
    this->size = 0;

}

bool string::
empty() const
{

    if (this->buffer == nullptr)
        return true;
    else if (this->size == 0)
        return true;
    else if (this->buffer[0] == '\0')
        return true;

    return false;

}

bool string::
find_last_of(ccptr characters, u64 &index) const
{

    u64 length = intrinsic_string_length(this->buffer);
    for (u64 i = length - 1; i > 0; i--)
    {

        for (u64 j = 0; characters[j] != '\0'; j++)
        {

            if (this->buffer[i] == characters[j])
            {

                index = i;
                return true;

            }

        }

    }

    return false;

}

string string::
substr(u64 end) const
{

    ENSURE(end < this->size);
    string str;
    str.resize(end + 1);
    intrinsic_string_copy_simple(str.buffer, this->buffer, end);
    return str;

}

string string::
substr(u64 start, u64 end) const
{

    ENSURE(start < this->size);
    ENSURE(end < this->size);
    ENSURE(start < end);
    string str;
    str.resize(end - start + 1);
    intrinsic_string_copy_simple(str.buffer, this->buffer + start, end - start);
    return str;

}

std::ostream& operator<<(std::ostream &stream, const string &str)
{

    stream << str.c_str();
    return stream;

}

string 
operator+(ccptr lhs, const string &rhs)
{

    string result;
    u64 length = intrinsic_string_length(lhs) + rhs.size + 1;
    result.resize(length);
    intrinsic_string_copy_simple(result.buffer, lhs, length);
    intrinsic_string_copy_simple(result.buffer + intrinsic_string_length(lhs), rhs.buffer, rhs.size);
    return result;

}

string 
operator+(const string &lhs, ccptr rhs)
{

    string result;
    u64 length = lhs.size + intrinsic_string_length(rhs) + 1;
    result.resize(length);
    intrinsic_string_copy_simple(result.buffer, lhs.buffer, lhs.size);
    intrinsic_string_copy_simple(result.buffer + lhs.size, rhs, length);
    return result;

}

#endif
