#ifndef SOURCE_COMPILER_LEXER_SCANNER_HPP
#define SOURCE_COMPILER_LEXER_SCANNER_HPP
#include <definitions.hpp>
#include <utilities/buffer.hpp>
#include <compiler/lexer/token.hpp>

struct scanner 
{

    memory_buffer source_buffer;

    string  source_file_path;
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

bool    scanner_initialize(scanner *lexer, ccptr file_path, u64 file_size);
string  scanner_get_last_error(scanner *lexer);

token   scanner_get_previous_token(scanner *lexer);
token   scanner_get_current_token(scanner *lexer);
token   scanner_get_next_token(scanner *lexer);

void    scanner_shift(scanner *lexer);

bool    scanner_previous_token_is(scanner *lexer, tokentype type);
bool    scanner_current_token_is(scanner *lexer, tokentype type);
bool    scanner_next_token_is(scanner *lexer, tokentype type);
bool    scanner_is_eof(scanner *lexer);

#endif