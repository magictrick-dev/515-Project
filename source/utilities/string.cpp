#include <utilities/string.hpp>
#include <utilities/allocators.hpp>
#include <utilities/intrinsics.hpp>

string::
string()
{

}

string::
string(ccptr str)
{

    u64 length = intrinsic_string_length(str) + 1;
    this->resize(size);
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
    intrinsic_string_copy_simple(this->buffer + this->size, str, length);

    return *this;

}

string& string::
operator+=(const string &str)
{

    u64 length = str.size;
    u64 new_size = this->size + length;
    this->resize(new_size);
    intrinsic_string_copy_simple(this->buffer + this->size, str.buffer, length);

    return *this;

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
    if (this->buffer != nullptr)
        intrinsic_string_copy_simple(buffer, this->buffer, size);

    this->clear();
    this->buffer = buffer;
    this->size = size;

}

void string::
clear()
{

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

std::ostream& operator<<(std::ostream &stream, const string &str)
{

    stream << str.c_str();
    return stream;

}
