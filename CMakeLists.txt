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

    ${PLATFORM_FILES}
)

TARGET_INCLUDE_DIRECTORIES(ncc PUBLIC "source")
