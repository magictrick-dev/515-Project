#include <iostream>
#include <string>
#include <definitions.hpp>
#include <buffer.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>
#include <compiler/sourcebuffer.hpp>

int
main(int argc, char ** argv)
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    ccptr file_name = argv[1];

    SourceBuffer user_source;
    if (!user_source.load(file_name))
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        std::cout << "Failed to load file: " << file_name << std::endl;
        return 1;
    }

    std::string output;
    while (!user_source.is_eof())
    {

        char c;
        if (!user_source.get(&c))
        {
            std::cout << "Failed to get character." << std::endl;
            return 1;
        }

        output += c;

    }

    i32 current_line = user_source.get_current_line();
    i32 current_column = user_source.get_current_column();
    u64 cursor = user_source.get_cursor();

    std::cout << "File: " << file_name << std::endl;
    std::cout << "Lines: " << current_line << std::endl;
    std::cout << "Columns: " << current_column << std::endl;
    std::cout << "Cursor: " << cursor << std::endl;

    // Unget until the beginning of the file.
    while (!user_source.is_sof())
    {
        char c;
        if (!user_source.unget(&c))
        {
            std::cout << "Failed to unget character." << std::endl;
            return 1;
        }
    }

    current_line = user_source.get_current_line();
    current_column = user_source.get_current_column();
    cursor = user_source.get_cursor();

    std::cout << "Lines: " << current_line << std::endl;
    std::cout << "Columns: " << current_column << std::endl;
    std::cout << "Cursor: " << cursor << std::endl;

    // Get the line string.
    std::string line = user_source.get_line_string();
    std::cout << "Line: " << line << std::endl;

    // Move until we reach the next line.
    while (!user_source.is_eof())
    {
        char c;
        if (!user_source.advance())
        {
            std::cout << "Failed to advance." << std::endl;
            return 1;
        }

        if (!user_source.peek(&c))
        {
            std::cout << "Failed to peek." << std::endl;
            return 1;
        }

        if (c == '\n')
            break;

    }

    user_source.advance();
    user_source.advance();
    user_source.advance();
    user_source.advance();

    // Get the line string.
    current_line = user_source.get_current_line();
    current_column = user_source.get_current_column();
    cursor = user_source.get_cursor();

    std::cout << "Lines: " << current_line << std::endl;
    std::cout << "Columns: " << current_column << std::endl;
    std::cout << "Cursor: " << cursor << std::endl;

    line = user_source.get_line_string();
    std::cout << "Line: " << line << std::endl;

    // Print each line.
    user_source.reset();

    while (!user_source.is_eof())
    {

        if (user_source.is_eol())
        {
            string line = user_source.get_line_string();
            std::cout << line << std::endl;
        }

        user_source.advance();

    }

    return 0;
}


