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

std::ostream& operator<<(std::ostream &stream, const string &str)
{

    stream << str.c_str();
    return stream;

}
