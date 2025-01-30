#include <compiler/lexer.hpp>
#include <platform/filesystem.hpp>
#include <platform/system.hpp>

static inline bool lexer_advance(scanner *lexer);
static inline bool lexer_consume(scanner *lexer, char *c);
static inline bool lexer_examine(scanner *lexer, char *c);
static inline bool lexer_peek(scanner *lexer, char *c);
static inline bool lexer_peek_ahead(scanner *lexer, u64 step, char *c);
template <typename ...Args> inline bool scanner_match(scanner *lexer, Args... args);

// --- Lexer Implementation ----------------------------------------------------
//
// Core lexer implementation is here. The lexer must provide the source buffer
// manually, and from there the initialization routine properly sets up the remainder
// of the lexer. This setup is effectively a rolling buffer of tokens, where each shift
// of the lexer advances the token buffer by one. This allows for a lookahead of 2 tokens,
// which may prove useful in parsing.
//
// lexer_advance(scanner *lexer) 
//      - Advances the lexer by one character.
// lexer_consume(scanner *lexer, char *c) 
//      - Consumes the current character and returns it.
// lexer_examine(scanner *lexer, char *c) 
//      - Examines the current character without consuming it.
// lexer_peek(scanner *lexer, char *c) 
//      - Peeks at the next character without consuming it.
// lexer_peek_ahead(scanner *lexer, u64 step, char *c) 
//      - Peeks at the character at the given step without consuming it.
// scanner_match(scanner *lexer, Args... args) 
//      - Matches the current character against the given arguments.
//
// This implementation is slightly different from the unget() system, but functionally
// works the same by allowing the lexer to peek forward rather than unwinding what 
// occured before.
//

bool    
lexer_initialize(scanner *lexer, ccptr file_path, u64 file_size)
{

    ENSURE_PTR(lexer);
    ENSURE_PTR(lexer->source_buffer.data);

    lexer->source_string    = (cptr)(lexer->source_buffer.data);
    lexer->source_size      = file_size;
    lexer->source_offset    = 0;
    lexer->source_step      = 0;
    lexer->source_line      = 1;
    lexer->source_column    = 1;

    lexer->previous_token   = &lexer->tokens[0];
    lexer->current_token    = &lexer->tokens[1];
    lexer->next_token       = &lexer->tokens[2];

    lexer->last_error       = "";

    return false;
}

bool    
lexer_release(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

string  
lexer_get_last_error(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return "";
}

token   
lexer_get_previous_token(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return {};
}

token   
lexer_get_current_token(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return {};
}

token   
lexer_get_next_token(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return {};
}

bool    
lexer_shift(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

bool    
lexer_previous_token_is(scanner *lexer, token_type type)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

bool    
lexer_current_token_is(scanner *lexer, token_type type)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

bool    
lexer_next_token_is(scanner *lexer, token_type type)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

bool    
lexer_is_eof(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

// --- Internal scanner Implementation -------------------------------------------
//
// Any functions needed by the API not provided for the frontend are written here.
//

static inline bool 
lexer_advance(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_consume(scanner *lexer, char *c)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_examine(scanner *lexer, char *c)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_peek(scanner *lexer, char *c)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_peek_ahead(scanner *lexer, u64 step, char *c)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

template <typename ...Args> inline bool 
scanner_match(scanner *lexer, Args... args)
{

    NOIMPL("Haven't done it yet.");
    return false;
}


