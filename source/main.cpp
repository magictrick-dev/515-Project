// --- NIU Compiler Class / NCC ------------------------------------------------
//
// Developed by Christopher DeJong / Z1836870@students.niu.edu
//
// NCC is a compiler written for CSCI-490D1 / CSCI-515 course. Below are the list
// of features of this compiler.
//
//      -   Memory buffer provides a unified interface to interact with source files.
//      -   Scanner/lexer provides the tokenization routines.
//
// Chances are, if you're reading this, then you're either my professor or a
// curious Github user. As you probably already can tell, there is a little bit
// more going on than what was outlined in the assignments' rubrics. I intend
// to make this one of my portfolio projects, so I used some of the techniques
// that I employ while working on my hobby projects in this project.
// 
//      -   I use virtual allocation over malloc/new for file operations and
//          custom memory allocators.
//
//      -   The standard library is fairly inconsistent across various platforms
//          (and even Linux distributions) so I write most of routines from scratch
//          to prevent weird compilation and runtime bugs between each platform.
//
//      -   Any code directly related to the assignments are usually written in
//          source/compiler/.
//
// -----------------------------------------------------------------------------
#include <iostream>
#include <definitions.hpp>
#include <platform/system.hpp>
#include <platform/filesystem.hpp>
#include <compiler/lexer/scanner.hpp>
#include <utilities/buffer.hpp>
#include <utilities/allocators.hpp>

int
main(int argc, char ** argv)
{

    // --- CLI Validation ------------------------------------------------------
    //
    // Checks the CLI arguments and configures the program accordingly.
    //

    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file_name>" << std::endl;
        return 1;
    }

    // --- Compiler Setup ------------------------------------------------------
    //
    // Initializes the compiler and prepares the source file for processing.
    //

    {

#if 0
        // Get the user source file.
        filepath source_file = system_get_current_working_directory();
        source_file += "/" + string(argv[1]);

        // Get the canonical path.
        filepath canonical_source_file = source_file.canonicalize();
        if (!canonical_source_file.is_file())
        {
            std::cout << "Invalid file path: " << canonical_source_file.str() << std::endl;
            return 1;
        }

        // Read the entire file into memory.
        u64 source_size = canonical_source_file.get_file_size();
#endif
        u64 source_size;
        filesystem_get_file_size(argv[1], &source_size);
        if (source_size == 0)
        {
            std::cout << "Failed to get file size: " << argv[1] << std::endl;
            std::cout << "File may not exist." << std::endl;
            return 1;
        }

        memory_buffer source_file_buffer = system_virtual_allocate(nullptr, source_size);
        if (!filesystem_read_entire_file(argv[1], &source_file_buffer))
        {
            std::cout << "Failed to read file: " << argv[1] << std::endl;
            return 1;
        }

        // Initialize the scanner.
        scanner lexer = {0};
        lexer.source_buffer = source_file_buffer;
        scanner_initialize(&lexer, argv[1], source_size);

        // Tokenize the source file.
        while (!scanner_is_eof(&lexer))
        {
            std::cout << lexer.current_token->format() << std::endl;
            scanner_shift(&lexer);
        }

        std::cout << lexer.current_token->format() << std::endl;

    }

    // --- Memory Check --------------------------------------------------------
    //
    // Checks the memory useage of the entire program.
    //

#if 0

    memory_stats stats = {0};
    memory_statistics(&stats);
    std::cout << "Total: " << stats.total_allocated << std::endl;
    std::cout << "Released: " << stats.total_released << std::endl;
    std::cout << "Peak: " << stats.peak_allocated << std::endl;
    std::cout << "Current: " << stats.current_allocated << std::endl;

#endif

    return 0;
}


