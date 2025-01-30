#include <iostream>
#include <definitions.hpp>
#include <buffer.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>
#include <compiler/lexer.hpp>

int
main(int argc, char ** argv)
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    std::cout << "Hello, world." << std::endl;
    ccptr file_name = argv[1];
    ccptr executable_directory = system_get_executable_directory();
    ccptr current_working_directory = system_get_current_working_directory();

    std::cout << "Executable directory: " << executable_directory << std::endl;
    std::cout << "Current working directory: " << current_working_directory << std::endl;

    return 0;
}


