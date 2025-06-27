#ifndef PLATFORM_WIN32_ENTRYPOINT_HPP
#define PLATFORM_WIN32_ENTRYPOINT_HPP
#include <windows.h>
#include <runtime.hpp>

int main(int argc, char ** argv)
{

    // Ever heard a grown man scream?
    SetConsoleOutputCP(CP_UTF8);
    int result = runtime(argc, argv);
    return result;

}

#endif
