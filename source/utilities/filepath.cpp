#include <filesystem>
#include <utilities/filepath.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>

filepath::
filepath()
{

}

filepath::
filepath(ccptr path)
{
    file_path = path;
}

filepath::
filepath(string path)
{
    file_path = path;
}

filepath::
filepath(const filepath& path)
{
    file_path = path.file_path;
}

filepath::
~filepath()
{

}

filepath& filepath::
operator=(const filepath& path)
{
    file_path = path.file_path;
    return *this;
}

filepath& filepath::
operator=(ccptr path)
{
    file_path = path;
    return *this;
}

filepath& filepath::
operator=(string path)
{
    file_path = path;
    return *this;
}

bool filepath::
operator==(const filepath& path) const
{
    return file_path == path.file_path;
}

bool filepath::
operator==(ccptr path) const
{
    return file_path == path;
}

bool filepath::
operator==(string path) const
{
    return file_path == path;
}

bool filepath::
operator!=(const filepath& path) const
{
    return file_path != path.file_path;
}

bool filepath::
operator!=(ccptr path) const
{
    return file_path != path;
}

bool filepath::
operator!=(string path) const
{
    return file_path != path;
}

filepath& filepath::
operator+=(ccptr path)
{
    file_path += path;
    return *this;
}

filepath& filepath::
operator+=(string path)
{
    file_path += path;
    return *this;
}

filepath& filepath::
operator+=(const filepath& path)
{
    file_path += path.file_path;
    return *this;
}

bool filepath::
is_empty() const
{
    return file_path.empty();
}

bool filepath::
is_valid() const
{
    return filesystem_path_is_valid(file_path.c_str());
}

bool filepath::
is_file() const
{
    return filesystem_file_exists(file_path.c_str());
}

bool filepath::
is_directory() const
{
    return filesystem_path_exists(file_path.c_str());
}

string filepath::
str() const
{
    return file_path;
}

ccptr filepath::
c_str() const
{
    return file_path.c_str();
}

string filepath::
get_directory() const
{

    string directory;
    u64 position = 0;
    if (file_path.find_last_of("/\\", position))
    {
        directory = file_path.substr(0, position);
    }
    
    return directory;

}

string filepath::
get_filename() const
{

    string filename;
    u64 position = 0;
    if (file_path.find_last_of("/\\", position))
    {
        filename = file_path.substr(position + 1);
    }

    return filename;

}

string filepath::
get_extension() const
{

    string extension;
    u64 position = 0;
    if (file_path.find_last_of(".", position))
    {
        extension = file_path.substr(position + 1);
    }

    return extension;

}

void filepath::
append(ccptr path)
{
    file_path += path;
}

void filepath::
append(string path)
{
    file_path += path;
}

void filepath::
append(const filepath& path)
{
    file_path += path.file_path;
}

filepath filepath::
canonicalize() const
{

    // Ew, but I am way too lazy to write this function.
    try {
        return std::filesystem::weakly_canonical(this->file_path.c_str()).string().c_str();
    } catch (const std::filesystem::filesystem_error& e) {
        return this->file_path;
    }
}

u64 filepath::
get_file_size() const
{

    u64 file_size;
    if (!filesystem_get_file_size(file_path.c_str(), &file_size))
    {
        return 0;
    }
    return file_size;

}

