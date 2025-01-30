#ifndef SOURCE_COMPILER_LEXER_HPP
#define SOURCE_COMPILER_LEXER_HPP
#include <definitions.hpp>
#include <buffer.hpp>
#include <compiler/errors.hpp>

enum class token_type
{
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

struct token 
{
    token_type  type;
    string      what;
    string      where;
    i32         line;
    i32         column;
};

struct scanner 
{

    memory_buffer source_buffer;

    cptr    source_string;
    u64     source_size;
    u64     source_offset;
    u64     source_step;
    u64     source_line;
    u64     source_column;

    token   tokens[3];
    token*  previous_token;
    token*  current_token;
    token*  next_token;

    string  last_error;
};

bool    lexer_initialize(scanner *lexer, ccptr file_path);
bool    lexer_release(scanner *lexer); 
string  lexer_get_last_error(scanner *lexer);

token   lexer_get_previous_token(scanner *lexer);
token   lexer_get_current_token(scanner *lexer);
token   lexer_get_next_token(scanner *lexer);

bool    lexer_shift(scanner *lexer);

bool    lexer_previous_token_is(scanner *lexer, token_type type);
bool    lexer_current_token_is(scanner *lexer, token_type type);
bool    lexer_next_token_is(scanner *lexer, token_type type);

bool    lexer_is_eof(scanner *lexer);

string  lexer_token_type_to_string(token_type type);

#endif