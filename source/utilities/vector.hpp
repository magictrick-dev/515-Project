#ifndef SOURCE_UTILITIES_VECTOR_HPP
#define SOURCE_UTILITIES_VECTOR_HPP
#include <definitions.hpp>

// --- Vector ------------------------------------------------------------------
//
// A vector class to replace the standard library implementation with a custom
// vector implementation that uses the allocator wrapper.
//

template <class type>
class vector
{

    public:
        inline          vector();    
        inline          vector(const vector& instance);
        inline          vector(vector&& instance);
        inline virtual ~vector();

    protected:
        inline void     resize(u64 size);
        inline void     clear();

    protected:
        type*   buffer;
        u64     size;

};

#endif
