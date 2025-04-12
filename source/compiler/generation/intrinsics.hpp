#ifndef COMPILER_GENERATION_INTRINSICS_HPP
#define COMPILER_GENERATION_INTRINSICS_HPP
#include <iostream>

inline void print_int(int value)
{

    printf("%i", value);

}

inline void print_bool(int value)
{

    printf("%i", value);

}

inline void print_string(const char *value)
{

    printf("%s", value);

}

int read_int(void)
{

    int value;
    std::cin >> value;
    return value;

}

#endif
