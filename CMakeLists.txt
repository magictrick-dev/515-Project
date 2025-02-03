CMAKE_MINIMUM_REQUIRED(VERSION 3.21)

PROJECT(ncc)
SET(CMAKE_CXX_STANDARD 17)

# Enable compile_commands.json
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

SET(PLATFORM_FILES "")

# Unix specific platform files.
IF(UNIX)
    SET(PLATFORM_FILES
        "source/platform/unix/system.cpp"
        "source/platform/unix/filesystem.cpp"
    )
ENDIF(UNIX)

# Windows specific platform files.
IF(WIN32)
    SET(PLATFORM_FILES
        "source/platform/win32/system.cpp"
        "source/platform/win32/filesystem.cpp"
    )
ENDIF(WIN32)

ADD_EXECUTABLE(ncc
    "source/main.cpp"
    "source/definitions.hpp"

    "source/utilities/buffer.hpp"
    "source/utilities/buffer.cpp"
    "source/utilities/filepath.hpp"
    "source/utilities/filepath.cpp"

    "source/compiler/errors.hpp"
    "source/compiler/lexer.hpp"
    "source/compiler/lexer.cpp"

    "source/platform/filesystem.hpp"
    "source/platform/system.hpp"

    ${PLATFORM_FILES}
)

TARGET_INCLUDE_DIRECTORIES(ncc PUBLIC "source")
