#include <compiler/lexer.hpp>
#include <platform/filesystem.hpp>
#include <platform/system.hpp>

static inline bool lexer_advance(scanner *lexer);
static inline bool lexer_consume(scanner *lexer, char *c);
static inline bool lexer_examine(scanner *lexer, char *c);
static inline bool lexer_peek(scanner *lexer, char *c);
static inline bool lexer_peek_ahead(scanner *lexer, u64 step, char *c);
static inline void lexer_synchronize(scanner *lexer);
template <typename ...Args> inline bool lexer_match(scanner *lexer, Args... args);

static inline bool lexer_consume_whitespace(scanner *lexer);
static inline bool lexer_match_symbols(scanner *lexer);
static inline bool lexer_match_numbers(scanner *lexer);
static inline bool lexer_match_strings(scanner *lexer);
static inline bool lexer_match_identifiers(scanner *lexer);
static inline void lexer_match_token(scanner *lexer);

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

    lexer->source_file_path = file_path;
    lexer->source_string    = (cptr)(lexer->source_buffer.data);
    lexer->source_size      = file_size;
    lexer->source_offset    = 0;
    lexer->source_step      = 0;
    lexer->source_line      = 1;
    lexer->source_column    = 1;

    lexer->previous_token   = &lexer->tokens[0];
    lexer->current_token    = &lexer->tokens[1];
    lexer->next_token       = &lexer->tokens[2];

    lexer->previous_token->type = token_type::TOKEN_EOF;
    lexer->current_token->type  = token_type::TOKEN_EOF;
    lexer->next_token->type     = token_type::TOKEN_EOF;

    lexer->last_error       = "";

    return false;

}

string  
lexer_get_last_error(scanner *lexer)
{

    return lexer->last_error;

}

token   
lexer_get_previous_token(scanner *lexer)
{

    return *lexer->previous_token;

}

token   
lexer_get_current_token(scanner *lexer)
{

    return *lexer->current_token;

}

token   
lexer_get_next_token(scanner *lexer)
{

    return *lexer->next_token;

}

void
lexer_shift(scanner *lexer)
{

    token *placeholder = lexer->previous_token;
    lexer->previous_token = lexer->current_token;
    lexer->current_token = lexer->next_token;
    lexer->next_token = placeholder;

    lexer_match_token(lexer);

}

bool    
lexer_previous_token_is(scanner *lexer, token_type type)
{

    if (lexer->previous_token->type == type)
        return true;
    return false;
}

bool    
lexer_current_token_is(scanner *lexer, token_type type)
{

    if (lexer->current_token->type == type)
        return true;
    return false;

}

bool    
lexer_next_token_is(scanner *lexer, token_type type)
{

    if (lexer->next_token->type == type)
        return true;
    return false;
}

bool    
lexer_is_eof(scanner *lexer)
{

    if (lexer->current_token->type == token_type::TOKEN_EOF)
        return true;
    return false;

}

string
lexer_token_type_to_string(token_type type)
{

    switch (type)
    {
        case token_type::TOKEN_NULL:                return "TOKEN_NULL";
        case token_type::TOKEN_ERROR:               return "TOKEN_ERROR";
        case token_type::TOKEN_EOF:                 return "TOKEN_EOF";
        case token_type::TOKEN_PLUS:                return "TOKEN_PLUS";
        case token_type::TOKEN_MINUS:               return "TOKEN_MINUS";
        case token_type::TOKEN_MULTIPLY:            return "TOKEN_MULTIPLY";
        case token_type::TOKEN_DIVIDE:              return "TOKEN_DIVIDE";
        case token_type::TOKEN_IDENTIFIER:          return "TOKEN_IDENT";
        case token_type::TOKEN_EXPONENT:            return "TOKEN_EXPONENT";
        case token_type::TOKEN_LESS:                return "TOKEN_LESS";
        case token_type::TOKEN_LESS_EQUAL:          return "TOKEN_LESS_EQUAL";
        case token_type::TOKEN_GREATER:             return "TOKEN_GREATER";
        case token_type::TOKEN_GREATER_EQUAL:       return "TOKEN_GREATER_EQUAL";
        case token_type::TOKEN_EQUAL:               return "TOKEN_EQUAL";
        case token_type::TOKEN_NOT_EQUAL:           return "TOKEN_NOT_EQUAL";
        case token_type::TOKEN_ASSIGN:              return "TOKEN_ASSIGN";
        case token_type::TOKEN_NOT:                 return "TOKEN_NOT";
        case token_type::TOKEN_LEFT_PARENTHESES:    return "TOKEN_LEFT_PARENTHESES";
        case token_type::TOKEN_RIGHT_PARENTHESES:   return "TOKEN_RIGHT_PARENTHESES";
        case token_type::TOKEN_LEFT_BRACKET:        return "TOKEN_LEFT_BRACKET";
        case token_type::TOKEN_RIGHT_BRACKET:       return "TOKEN_RIGHT_BRACKET";
        case token_type::TOKEN_LEFT_BRACE:          return "TOKEN_LEFT_BRACE";
        case token_type::TOKEN_RIGHT_BRACE:         return "TOKEN_RIGHT_BRACE";
        case token_type::TOKEN_AND:                 return "TOKEN_AND";
        case token_type::TOKEN_OR:                  return "TOKEN_OR";
        case token_type::TOKEN_DOT:                 return "TOKEN_DOT";
        case token_type::TOKEN_AT:                  return "TOKEN_AT";
        case token_type::TOKEN_INTEGER:             return "TOKEN_INTEGER";
        case token_type::TOKEN_STRING:              return "TOKEN_STRING";
        case token_type::TOKEN_COLON:               return "TOKEN_COLON";
        case token_type::TOKEN_SEMICOLON:           return "TOKEN_SEMICOLON";
        case token_type::TOKEN_COMMA:               return "TOKEN_COMMA";
        case token_type::TOKEN_REAL:                return "TOKEN_REAL";
        default: NOREACH("Unknown token type.");
    }

    return ""; // Otherwise the compiler will throw a fit.

}

// --- Internal scanner Implementation -------------------------------------------
//
// Any functions needed by the API not provided for the frontend are written here.
//

static inline bool 
lexer_advance(scanner *lexer)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    lexer->source_step++;
    return true;

}

static inline bool 
lexer_consume(scanner *lexer, char *c)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step];
    lexer->source_step++;
    return true;

}

static inline bool 
lexer_examine(scanner *lexer, char *c)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step];
    return true;

}

static inline bool 
lexer_peek(scanner *lexer, char *c)
{

    if (lexer->source_step+1 >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step+1];
    return true;

}

static inline bool 
lexer_peek_ahead(scanner *lexer, u64 step, char *c)
{

    if (lexer->source_step+step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step+step];
    return true;

}

template <typename ...Args> inline bool 
lexer_match(scanner *lexer, Args... args)
{

    char current_character;
    if (!lexer_examine(lexer, &current_character))
        return false;

    for (auto c : { args... })
    {
        if (current_character == c)
            return true;
    }

    return false;

}

static inline bool 
lexer_consume_whitespace(scanner *lexer)
{

    if (lexer_match(lexer, ' ', '\n', '\r', '\t'))
    {
        lexer_advance(lexer);
        return true;
    }

    // TODO(Chris): Handle comments. 
    //              They are '<<-' to '->>' and line comments are '#'.
    //              Handle within strings?

    return false;

}

static inline void
scanner_synchronize(scanner *lexer)
{

    lexer->source_offset = lexer->source_step;
    return;

}

static inline bool 
lexer_match_symbols(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_match_numbers(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_match_strings(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline bool 
lexer_match_identifiers(scanner *lexer)
{

    NOIMPL("Haven't done it yet.");
    return false;
}

static inline void
lexer_match_token(scanner *lexer)
{
    
    // Consume all whitespace.
    while (lexer_consume_whitespace(lexer));

    // If we are at the end, just return an EOF token.
    if (lexer->source_step >= lexer->source_size)
    {
        lexer->next_token->type      = token_type::TOKEN_EOF;
        lexer->next_token->where     = lexer->source_file_path;
        lexer->next_token->column    = lexer->source_column;
        lexer->next_token->line      = lexer->source_line;
        lexer->next_token->what      = "";
        return;
    }

    if (lexer_match_symbols(lexer))     return;
    if (lexer_match_numbers(lexer))     return;
    if (lexer_match_strings(lexer))     return;
    if (lexer_match_identifiers(lexer)) return;

    // If we reach here, we have an error.
    lexer->last_error = "Unknown token.";
    char c;
    lexer_consume(lexer, &c);


    return;

}


