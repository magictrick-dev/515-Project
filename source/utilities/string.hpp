#ifndef SOURCE_UTILITIES_STRING_HPP
#define SOURCE_UTILITIES_STRING_HPP
#include <cstdint>

class string
{
    public:
                    string();
                    string(const char *str);
                    string(const string &str);
                    string(string &&str);
        virtual    ~string();

        string&     operator=(const char *str);
        string&     operator=(const string &str);
        string&     operator=(string &&str);

    protected:
        char *buffer;
        uint64_t length;
};

#endif
