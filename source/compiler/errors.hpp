#ifndef SOURCE_COMPILER_ERRORS_HPP
#define SOURCE_COMPILER_ERRORS_HPP
#include <definitions.hpp>
#include <utilities/string.hpp>

enum class ErrorType : i32
{

    None                        = 0,

    ArgumentCount               = 1,
    ArgumentInvalidType         = 2,
    ArgumentInvalidValue        = 3,

    FileNotFound                = 10,
    FileNotOpened               = 11,
    
    SourceBufferEOF             = 20,
    SourceBufferSOF             = 21,
    SourceBufferEOL             = 22,

    LexerInvalidToken           = 100,

};

struct Error
{

    ErrorType type;
    i32 line;
    i32 column;

};

inline string
translate_error_type(const Error& error)
{

    switch (error.type)
    {

        case ErrorType::None:
            return "";
        case ErrorType::ArgumentCount:
            return "Invalid number of arguments";
        case ErrorType::ArgumentInvalidType:
            return "Invalid command line argument type encountered";
        case ErrorType::ArgumentInvalidValue:
            return "Invalid command line argument value";

        case ErrorType::FileNotFound:
            return "Error opening file, file not found";
        case ErrorType::FileNotOpened:
            return "Error opening file, file could not be opened";

        case ErrorType::SourceBufferEOF:
            return "Reached unexpected end of file";
        case ErrorType::SourceBufferSOF:
            return "Reached unexpected start of file";
        case ErrorType::SourceBufferEOL:
            return "Reached unexpected end of line";

        case ErrorType::LexerInvalidToken:
            return "Unexpected token encountered";

    }

    NOREACH("Unimplemented error case.");  
    return "";

}

#endif
