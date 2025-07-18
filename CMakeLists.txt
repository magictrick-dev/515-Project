CMAKE_MINIMUM_REQUIRED(VERSION 3.21)

PROJECT(ncc)
SET(CMAKE_CXX_STANDARD 17)

# Enable compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE Debug)

SET(PLATFORM_FILES "")

# Unix specific platform files.
IF(UNIX)
    MESSAGE("Using UNIX")
    SET(PLATFORM_FILES
        "source/platform/unix/entrypoint.cpp"
        "source/platform/unix/system.cpp"
        "source/platform/unix/filesystem.cpp"
    )
ENDIF(UNIX)

# Windows specific platform files.
IF(WIN32)
    MESSAGE("Using WIN32")
    SET(PLATFORM_FILES
        "source/platform/win32/entrypoint.cpp"
        "source/platform/win32/system.cpp"
        "source/platform/win32/filesystem.cpp"
    )
ENDIF(WIN32)

# The executable.
ADD_EXECUTABLE(ncc
    ${PLATFORM_FILES}
    
    "source/runtime.hpp"
    "source/runtime.cpp"
    "source/definitions.hpp"

    "source/utilities/buffer.hpp"
    "source/utilities/buffer.cpp"
    "source/utilities/filepath.hpp"
    "source/utilities/filepath.cpp"
    "source/utilities/allocators.hpp"
    "source/utilities/allocators.cpp"
    "source/utilities/string.hpp"
    "source/utilities/string.cpp"
    "source/utilities/intrinsics.hpp"
    "source/utilities/intrinsics.cpp"

    "source/compiler/compiler.hpp"
    "source/compiler/compiler.cpp"
    "source/compiler/graph.hpp"
    "source/compiler/graph.cpp"
    "source/compiler/environment.hpp"
    "source/compiler/environment.cpp"
    "source/compiler/reference.hpp"
    "source/compiler/reference.cpp"

    "source/compiler/lexer/token.hpp"
    "source/compiler/lexer/token.cpp"
    "source/compiler/lexer/tokenizer.hpp"
    "source/compiler/lexer/tokenizer.cpp"
    
    "source/compiler/parser/node.hpp"
    "source/compiler/parser/node.cpp"
    "source/compiler/parser/subnodes.hpp"
    "source/compiler/parser/subnodes.cpp"
    "source/compiler/parser/parser.hpp"
    "source/compiler/parser/parser.cpp"
    "source/compiler/parser/visitor.hpp"
    "source/compiler/parser/visitor.cpp"
    
    "source/compiler/generation/codegen.hpp"
    "source/compiler/generation/codegen.cpp"

    "source/platform/filesystem.hpp"
    "source/platform/system.hpp"

)

# Include directories.
TARGET_INCLUDE_DIRECTORIES(ncc PUBLIC "source")
