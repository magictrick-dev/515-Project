#ifndef SOURCE_COMPILER_LEXER_HPP
#define SOURCE_COMPILER_LEXER_HPP
#include <definitions.hpp>
#include <compiler/sourcebuffer.hpp>
#include <compiler/errors.hpp>

enum class TokenType
{
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_FORWARD_SLASH,
};

struct Token 
{
    TokenType   type;
    string      what;
    string      where;
    i32         line;
    i32         column;
};

struct Lexer
{
    cptr    source_buffer;
    u64     source_size;
    u64     source_offset;
    u64     source_step;

    Token   tokens[3];
    Token*  previous_token;
    Token*  current_token;
    Token*  next_token;

    string  last_error;
};

bool    lexer_initialize(Lexer *lexer, ccptr file_path);
bool    lexer_release(Lexer *lexer); 
string  lexer_get_last_error(Lexer *lexer);

Token   lexer_get_previous_token(Lexer *lexer);
Token   lexer_get_current_token(Lexer *lexer);
Token   lexer_get_next_token(Lexer *lexer);

bool    lexer_shift(Lexer *lexer);

bool    lexer_previous_token_is(Lexer *lexer, TokenType type);
bool    lexer_current_token_is(Lexer *lexer, TokenType type);
bool    lexer_next_token_is(Lexer *lexer, TokenType type);

bool    lexer_is_eof(Lexer *lexer);


#endif
