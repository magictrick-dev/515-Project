#ifndef SOURCE_COMPILER_LEXER_TOKEN_HPP
#define SOURCE_COMPILER_LEXER_TOKEN_HPP
#include <definitions.hpp>
#include <utilities/string.hpp>

enum class TokenType
{
    TOKEN_NULL,
    TOKEN_ERROR_UNKNOWN,
    TOKEN_ERROR_INVALID_INTEGER,
    TOKEN_ERROR_INVALID_REAL,
    TOKEN_ERROR_UNEXPECTED_EOF,
    TOKEN_ERROR_INVALID_ESCAPE,
    TOKEN_EOF,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_IDENTIFIER,
    TOKEN_EXPONENT,
    TOKEN_LESS,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER,
    TOKEN_GREATER_EQUAL,
    TOKEN_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_ASSIGN,
    TOKEN_NOT,
    TOKEN_LEFT_PARENTHESES,
    TOKEN_RIGHT_PARENTHESES,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_DOT,
    TOKEN_AT,
    TOKEN_INTEGER,
    TOKEN_STRING,
    TOKEN_COLON,
    TOKEN_SEMICOLON,
    TOKEN_COMMA,
    TOKEN_REAL
};

class Token
{

    public:
                    Token();
                    Token(string reference, string source, TokenType type, i32 line, i32 column);
        virtual    ~Token();

        string      get_reference() const;
        string      get_source() const;
        TokenType   get_type() const;
        string      get_type_string() const;
        i32         get_line() const;
        i32         get_column() const;

        string      format() const;

        string      parse_reference_as_string() const;

    protected:
        string      reference;
        string      source;
        TokenType   type;
        i32         line;
        i32         column;

};

#endif