#include <iostream>
#include <definitions.hpp>
#include <buffer.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>
#include <compiler/sourcebuffer.hpp>
#include <compiler/lexer.hpp>

int
main(int argc, char ** argv)
{

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }



    return 0;
}


