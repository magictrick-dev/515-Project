#ifndef SOURCE_UTILITIES_STRING_HPP
#define SOURCE_UTILITIES_STRING_HPP
#include <definitions.hpp>
#include <iostream>

// --- String ------------------------------------------------------------------
//
// Replaces the standard library implementation with one that interfaces directly
// with the custom allocator. Never trust the standard library. Bad things happen
// when you do.
//

class string
{
    public:
                    string();
                    string(ccptr str);
                    string(const string &str);
                    string(string &&str);
        virtual    ~string();

        string&     operator=(ccptr str);
        string&     operator=(const string &str);
        string&     operator=(string &&str);

        char&       operator[](u64 index);
        char        operator[](u64 index) const;

        cptr        data();
        ccptr       c_str() const;

    protected:
        void        resize(u64 size);
        void        clear();

    protected:
        cptr    buffer;
        u64     size;
};

std::ostream& operator<<(std::ostream &stream, const string &str);

#endif
