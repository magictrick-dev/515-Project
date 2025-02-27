#ifndef SOURCE_UTILITIES_STRING_HPP
#define SOURCE_UTILITIES_STRING_HPP
#include <definitions.hpp>
#include <iostream>
#include <string>

using std::string;

#if 0

// --- String ------------------------------------------------------------------
//
// It's no where near as fancy as the STL string class, but what it does do, it
// does poorly.
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

        bool        operator==(const string &str) const;
        bool        operator==(ccptr str) const;

        bool        operator!=(const string &str) const;
        bool        operator!=(ccptr str) const;

        string&     operator+=(ccptr str);
        string&     operator+=(const string &str);
        string&     operator+=(char c);
        string&     operator+=(int i);
        string&     operator+=(float i);
        string&     operator+=(double i);

        cptr        data();
        ccptr       c_str() const;

        u64         length() const;

        bool        empty() const;

        bool        find_last_of(ccptr characters, u64 &index) const;

        string      substr(u64 end) const;
        string      substr(u64 start, u64 end) const;

    protected:
        void        resize(u64 size);
        void        clear();

    protected:
        cptr    buffer  = nullptr;
        u64     size    = 0;

    friend std::ostream& operator<<(std::ostream &stream, const string &str);
    friend string operator+(ccptr lhs, const string &rhs);
    friend string operator+(const string &lhs, ccptr rhs);

};

std::ostream& operator<<(std::ostream &stream, const string &str);
string operator+(ccptr lhs, const string &rhs);
string operator+(const string &lhs, ccptr rhs);

#endif

#endif
