#ifndef COMPILER_GENERATION_INTRINSICS_HPP
#define COMPILER_GENERATION_INTRINSICS_HPP
#include <iostream>

inline void print_int(int value)
{

    std::cout << value;

}

inline void print_bool(int value)
{

    std::cout << ((value & 0xFF) ? "true" : "false");

}

inline void print_string(const char *value)
{

    std::cout << value;

}

int read_int(void)
{

    int value;
    std::cin >> value;
    return value;

}

#endif
