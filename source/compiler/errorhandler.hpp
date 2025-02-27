#ifndef SOURCE_COMPILER_ERRORHANDLER_HPP
#define SOURCE_COMPILER_ERRORHANDLER_HPP
#include <exception>
#include <definitions.hpp>
#include <utilities/string.hpp>

class CompilerException : public std::exception
{

    public:
        virtual ccptr what() const noexcept override = 0;
        
        static inline uint64_t get_exceptions_called() { return exceptions_called; }

    protected:
        static inline uint64_t exceptions_called = 0;

};

// [(line)] [resource][(line,column)]: [formatted message]

class CompilerResourceException : public CompilerException
{

    public:
        template <typename... Args>
        inline CompilerResourceException(uint64_t line_number, string resource_name, string format, Args... args)
        {

            this->message = "[" + std::to_string(line_number) + "]: " + resource_name + ": ";
            
            // NOTE(Chris): This is what we call in the industry as a "code smell".
            // TODO(Chris): Clearly there must be a better way to do this.
#           pragma warning(push)
#           pragma GCC diagnostic push
#           pragma GCC diagnostic ignored "-Wformat-security"
            i32 size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
            string formatted_message(size_s, ' ');
            std::snprintf(formatted_message.data(), size_s, format.c_str(), args...);
            formatted_message.pop_back();
#           pragma GCC diagnostic pop
#           pragma warning(pop)

            this->message += formatted_message;
            
            CompilerException::exceptions_called++;
            
        }

        inline virtual ccptr 
        what() const noexcept override
        {
            return this->message.c_str();
        }

    protected:
        std::string message;

};

class CompilerSyntaxErrorException : public CompilerException
{

    public:
        template <typename... Args>
        inline CompilerSyntaxErrorException(uint64_t line_number, string resource_name,
             uint64_t line, uint64_t column, string format, Args... args)
        {

            this->message = "[" + std::to_string(line_number) + "]: " + resource_name + " (" 
                + std::to_string(line) + "," + std::to_string(column) + "): ";

            // NOTE(Chris): This is what we call in the industry as a "code smell".
            // TODO(Chris): Clearly there must be a better way to do this.
#           pragma warning(push)
#           pragma GCC diagnostic push
#           pragma GCC diagnostic ignored "-Wformat-security"
            i32 size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
            string formatted_message(size_s, ' ');
            std::snprintf(formatted_message.data(), size_s, format.c_str(), args...);
            formatted_message.pop_back();
#           pragma GCC diagnostic pop
#           pragma warning(pop)

            this->message += formatted_message;

            CompilerException::exceptions_called++;
            
        }
        
        inline virtual ccptr
        what() const noexcept override
        {
            return this->message.c_str();
        }
        
    protected:
        std::string message;

};

#endif