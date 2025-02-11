#ifndef SOURCE_COMPILER_ENVIRONMENT_HPP
#define SOURCE_COMPILER_ENVIRONMENT_HPP
#include <definitions.hpp>

class Environment
{

    public:
                    Environment();
        virtual    ~Environment();

        void        push_table();
        void        pop_table();

    protected:
        
};

#endif
