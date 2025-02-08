#include <compiler/lexer/scanner.hpp>

static inline bool                          scanner_advance(scanner *lexer);
static inline bool                          scanner_consume(scanner *lexer, char *c);
static inline bool                          scanner_examine(scanner *lexer, char *c);
static inline bool                          scanner_peek(scanner *lexer, char *c);
static inline bool                          scanner_peek_ahead(scanner *lexer, u64 step, char *c);
static inline void                          scanner_synchronize(scanner *lexer);
template <typename ...Args> inline bool     scanner_match(scanner *lexer, Args... args);
static inline bool                          scanner_consume_whitespace(scanner *lexer);
static inline bool                          scanner_match_symbols(scanner *lexer);
static inline bool                          scanner_match_numbers(scanner *lexer);
static inline bool                          scanner_match_strings(scanner *lexer);
static inline bool                          scanner_match_identifiers(scanner *lexer);
static inline void                          scanner_match_token(scanner *lexer);
static inline token                         scanner_create_token(scanner *lexer, tokentype type);

bool    
scanner_initialize(scanner *lexer, ccptr file_path, u64 file_size)
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
    lexer->last_error       = "";

    scanner_shift(lexer);
    scanner_shift(lexer);

    return true;

}

string  
scanner_get_last_error(scanner *lexer)
{

    return lexer->last_error;

}

token   
scanner_get_previous_token(scanner *lexer)
{

    return *lexer->previous_token;

}

token   
scanner_get_current_token(scanner *lexer)
{

    return *lexer->current_token;

}

token   
scanner_get_next_token(scanner *lexer)
{

    return *lexer->next_token;

}

void
scanner_shift(scanner *lexer)
{

    token *placeholder = lexer->previous_token;
    lexer->previous_token = lexer->current_token;
    lexer->current_token = lexer->next_token;
    lexer->next_token = placeholder;

    scanner_match_token(lexer);

}

bool    
scanner_previous_token_is(scanner *lexer, tokentype type)
{

    if (lexer->previous_token->get_type() == type)
        return true;
    return false;
}

bool    
scanner_current_token_is(scanner *lexer, tokentype type)
{

    if (lexer->current_token->get_type() == type)
        return true;
    return false;

}

bool    
scanner_next_token_is(scanner *lexer, tokentype type)
{

    if (lexer->next_token->get_type() == type)
        return true;
    return false;
}

bool    
scanner_is_eof(scanner *lexer)
{

    if (lexer->current_token->get_type() == tokentype::TOKEN_EOF)
        return true;
    return false;

}

// --- Internal Scanner Implementation -------------------------------------------
//
// Any functions needed by the API not provided for the frontend are written here.
//

static inline bool 
scanner_advance(scanner *lexer)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    char c = lexer->source_string[lexer->source_step];
    if (c == '\n')
    {
        lexer->source_line++;
        lexer->source_column = 1;
    }
    else
    {
        lexer->source_column++;
    }

    lexer->source_step++;
    return true;

}

static inline bool 
scanner_consume(scanner *lexer, char *c)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step];
    if (*c == '\n')
    {
        lexer->source_line++;
        lexer->source_column = 1;
    }
    else
    {
        lexer->source_column++;
    }

    lexer->source_step++;
    return true;

}

static inline bool 
scanner_examine(scanner *lexer, char *c)
{

    if (lexer->source_step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step];
    return true;

}

static inline bool 
scanner_peek(scanner *lexer, char *c)
{

    if (lexer->source_step+1 >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step+1];
    return true;

}

static inline bool 
scanner_peek_ahead(scanner *lexer, u64 step, char *c)
{

    if (lexer->source_step+step >= lexer->source_size)
        return false;

    *c = lexer->source_string[lexer->source_step+step];
    return true;

}

template <typename ...Args> inline bool 
scanner_match(scanner *lexer, Args... args)
{

    char current_character;
    if (!scanner_examine(lexer, &current_character))
        return false;

    for (auto c : { args... })
    {
        if (current_character == c)
            return true;
    }

    return false;

}

static inline void
scanner_synchronize(scanner *lexer)
{

    lexer->source_offset = lexer->source_step;
    return;

}

static inline bool 
scanner_consume_whitespace(scanner *lexer)
{

    // White space characters.
    if (scanner_match(lexer, ' ', '\n', '\r', '\t'))
    {
        scanner_advance(lexer);
        return true;
    }

    // Single line comments.
    if (scanner_match(lexer, '#'))
    {

        scanner_advance(lexer);

        while (true)
        {

            char c;
            if (!scanner_consume(lexer, &c))
            {
                lexer->last_error = "Unexpected EOF.";
                return false;
            }

            if (c == '\n')
                break;

        }

        return true;

    }

    // Block comments begin with <<- and end with ->>.
    if (scanner_match(lexer, '<'))
    {

        char head;
        char body;
        char tail;

        scanner_examine(lexer, &head);
        scanner_peek(lexer, &body);
        scanner_peek_ahead(lexer, 2, &tail);

        if (head == '<' && body == '<' && tail == '-')
        {

            scanner_advance(lexer);
            scanner_advance(lexer);
            scanner_advance(lexer);

            while (true)
            {

                if (!scanner_consume(lexer, &head))
                {
                    lexer->last_error = "Unexpected EOF.";
                    return false;
                }

                if (!scanner_examine(lexer, &body))
                {
                    lexer->last_error = "Unexpected EOF.";
                    return false;
                }

                if (!scanner_peek(lexer, &tail))
                {
                    lexer->last_error = "Unexpected EOF.";
                    return false;
                }

                if (head == '-' && body == '>' && tail == '>')
                {
                    scanner_advance(lexer);
                    scanner_advance(lexer);
                    scanner_advance(lexer);
                    break;
                }

            }

            return true;

        }

    }

    return false;

}

static inline bool 
scanner_match_symbols(scanner *lexer)
{

    char c;
    if (!scanner_examine(lexer, &c)) return false; // EOF.

    switch (c)
    {
        case '+':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_PLUS);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '-':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_MINUS);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '*':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_MULTIPLY);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '/':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_DIVIDE);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '^':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_EXPONENT);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '<':
        {

            char peek;
            scanner_peek(lexer, &peek);

            if (peek == '=')
            {
                scanner_consume(lexer, &c);
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_LESS_EQUAL);
                scanner_synchronize(lexer);
                return true;
            }

            else if (peek == '-')
            {
                scanner_consume(lexer, &c);
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_ASSIGN);
                scanner_synchronize(lexer);
                return true;
            }

            else
            {
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_LESS);
                scanner_synchronize(lexer);
                return true;
            }

        } break;

        case '>':
        {

            char peek;
            scanner_peek(lexer, &peek);

            if (peek == '=')
            {
                scanner_consume(lexer, &c);
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_GREATER_EQUAL);
                scanner_synchronize(lexer);
                return true;
            }
            else
            {
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_GREATER);
                scanner_synchronize(lexer);
                return true;
            }

        } break;

        case '=':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_EQUAL);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '~':
        {

            // If we can't find a token, then this token is invalid.
            char peek;
            if (!scanner_peek(lexer, &peek))
                return false;

            // Must be an equals.
            if (peek == '=')
            {
                scanner_consume(lexer, &c);
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_NOT_EQUAL);
                scanner_synchronize(lexer);
                return true;
            }

            else
            {
                return false;
            }

        } break;

        case '!':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_NOT);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '(':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_LEFT_PARENTHESES);
            scanner_synchronize(lexer);
            return true;

        } break;

        case ')':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_RIGHT_PARENTHESES);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '[':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_LEFT_BRACKET);
            scanner_synchronize(lexer);
            return true;

        } break;

        case ']':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_RIGHT_BRACKET);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '{':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_LEFT_BRACE);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '}':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_RIGHT_BRACE);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '&':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_AND);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '|':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_OR);
            scanner_synchronize(lexer);
            return true;

        } break;

        case '.':
        {

            char peek;
            scanner_peek(lexer, &peek);

            // Handled by numbers.
            if (isdigit(peek))
            {
                return false;
            }

            // Otherwise, just a dot.
            else
            {
                scanner_consume(lexer, &c);
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_DOT);
                scanner_synchronize(lexer);
                return true;
            }

        } break;

        case '@':
        {

            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_AT);
            scanner_synchronize(lexer);
            return true;

        } break;

        case ':':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_COLON);
            scanner_synchronize(lexer);
            return true;

        } break;

        case ';':
        {

            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_SEMICOLON);
            scanner_synchronize(lexer);
            return true;

        }

        case ',':
        {
            
            scanner_consume(lexer, &c);
            *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_COMMA);
            scanner_synchronize(lexer);
            return true;

        } break;

    }

    return false;

}

static inline bool 
scanner_match_numbers(scanner *lexer)
{

    char initial_character;
    if (!scanner_examine(lexer, &initial_character)) return false; // EOF.

    // Must start with a number or a decimal.
    if (isdigit(initial_character) || initial_character == '.')
    {

        // Base case.
        bool exponentiated = false;
        tokentype type = tokentype::TOKEN_INTEGER;
        if (initial_character == '.') type = tokentype::TOKEN_REAL;

        scanner_advance(lexer);

        char check;
        while (true)
        {

            if (!scanner_examine(lexer, &check))
                break; // EOF.

            if (isdigit(check))
            {
                scanner_advance(lexer);
            }

            else if (check == '.')
            {

                // If we already encountered a decimal, break.
                if (type == tokentype::TOKEN_REAL)
                    break;

                // If the following character is an e/E it is not valid.
                char peek;
                if (!scanner_peek(lexer, &peek))
                {
                    type = tokentype::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

                if (peek == 'e' || peek == 'E')
                {
                    type = tokentype::TOKEN_ERROR_INVALID_REAL;
                    scanner_advance(lexer);
                }

                if (type == tokentype::TOKEN_INTEGER)
                    type = tokentype::TOKEN_REAL;
                scanner_advance(lexer);

            }

            else if (check == 'e' || check == 'E')
            {

                // The following character must either be a number or +/-.
                char peek;
                if (!scanner_peek(lexer, &peek))
                {
                    type = tokentype::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

                if (isdigit(peek))
                {
                    if (type == tokentype::TOKEN_INTEGER)
                        type = tokentype::TOKEN_REAL;
                    scanner_advance(lexer);
                    scanner_advance(lexer);
                    exponentiated = true;
                }

                else if (peek == '+' || peek == '-')
                {

                    // The following character must be a number.
                    char peek_ahead;
                    if (!scanner_peek_ahead(lexer, 2, &peek_ahead))
                    {
                        type = tokentype::TOKEN_ERROR_INVALID_REAL;
                        break;
                    }

                    if (isdigit(peek_ahead))
                    {
                        if (type == tokentype::TOKEN_INTEGER)
                            type = tokentype::TOKEN_REAL;
                        scanner_advance(lexer);
                        scanner_advance(lexer);
                        scanner_advance(lexer);
                        exponentiated = true;
                    }

                    else
                    {
                        type = tokentype::TOKEN_ERROR_INVALID_REAL;
                        scanner_advance(lexer);
                        scanner_advance(lexer);
                        break;
                    }

                }

                else
                {
                    type = tokentype::TOKEN_ERROR_INVALID_REAL;
                    scanner_advance(lexer);
                    break;
                }

            }

            else
            {
                break;
            }

        }

        // Integers can't start with zeros.
        if (type == tokentype::TOKEN_INTEGER && initial_character == '0')
            type = tokentype::TOKEN_ERROR_INVALID_INTEGER;

        *lexer->next_token = scanner_create_token(lexer, type);
        scanner_synchronize(lexer);
        return true;

    }

    return false;
}

static inline bool 
scanner_match_strings(scanner *lexer)
{

    char c;
    if (!scanner_examine(lexer, &c)) return false; // EOF.

    // Must start with a double quote.
    if (c == '"')
    {

        tokentype type = tokentype::TOKEN_STRING;
        scanner_advance(lexer);

        while (true)
        {

            char check;
            if (!scanner_examine(lexer, &check))
            {
                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_ERROR_UNEXPECTED_EOF);
                scanner_synchronize(lexer);
                return true;
            }

            if (check == '\\')
            {
                char peek;
                if (!scanner_peek(lexer, &peek))
                {
                    *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_ERROR_UNEXPECTED_EOF);
                    scanner_synchronize(lexer);
                    return true;
                }

                switch (peek)
                {
                    case 'n':   break;
                    case 't':   break;
                    case 'r':   break;
                    case '"':   break;
                    case '\\':  break;
                    case 'a':   break;
                    case 'b':   break;
                    case '\n':   break;

                    case 'u':
                    {
                        // Unicode escape. Check ahead for 6 characters.
                        char unicode[6];
                        for (u64 i = 0; i < 6; i++)
                        {
                            if (!scanner_peek_ahead(lexer, i+2, &unicode[i]))
                            {
                                *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_ERROR_INVALID_ESCAPE);
                            }
                        }

                        // Check if all characters are valid hex.
                        bool valid = true;
                        for (u64 i = 0; i < 6; i++)
                        {
                            if (!isxdigit(unicode[i]))
                            {
                                valid = false;
                                break;
                            }
                        }

                        if (valid)
                        {

                            // Manually shift 6 characters to compensate for the unicode escape.
                            scanner_advance(lexer);
                            scanner_advance(lexer);
                            scanner_advance(lexer);
                            scanner_advance(lexer);
                            scanner_advance(lexer);
                            scanner_advance(lexer);

                        }

                        else
                        {
                            type = tokentype::TOKEN_ERROR_INVALID_ESCAPE;
                        }

                    } break;

                    default:
                    {
                        type = tokentype::TOKEN_ERROR_INVALID_ESCAPE;
                    } break;
                }

                scanner_advance(lexer);
                scanner_advance(lexer);

            }

            else if (check == '\"')
            {
                scanner_advance(lexer);
                break;
            }

            else
            {
                scanner_advance(lexer);
            }

        }

        *lexer->next_token = scanner_create_token(lexer, type);
        scanner_synchronize(lexer);
        return true;

    }

    return false;
}

static inline bool 
scanner_match_identifiers(scanner *lexer)
{

    char c;
    if (!scanner_examine(lexer, &c)) return false; // EOF.

    // Must start with a letter, lowercase or uppercase is fine.
    if (isalpha(c) || c == '_')
    {

        scanner_advance(lexer);

        // Basically everything else can be a combination of letters and numbers.
        while (true)
        {

            char check;
            if (!scanner_examine(lexer, &check))
                break; // EOF.

            if (isalnum(check) || check == '_')
            {
                scanner_advance(lexer);
            }

            else
            {
                break;
            }

        }

        *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_IDENTIFIER);
        scanner_synchronize(lexer);
        return true;

    }

    return false;
}

static inline void
scanner_match_token(scanner *lexer)
{
    
    // Consume all whitespace.
    while (true)
    {

        bool consume_results = scanner_consume_whitespace(lexer);
        if (consume_results)
        {
            scanner_synchronize(lexer);
            continue;
        }
        else if (lexer->last_error != "")
        {
            *lexer->next_token = scanner_create_token(lexer, 
                tokentype::TOKEN_ERROR_UNEXPECTED_EOF);
            scanner_synchronize(lexer);
            lexer->last_error = ""; // Reset last error.
            return;
        }
        else
        {
            scanner_synchronize(lexer);
            break;
        }

    }

    if (lexer->last_error != "")
    {
        std::cout << lexer->last_error << std::endl;
        lexer->last_error = ""; // Reset the error.
    }

    // If we are at the end, just return an EOF token.
    if (lexer->source_step >= lexer->source_size)
    {
        *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_EOF);
        scanner_synchronize(lexer);
        return;
    }

    // Process all viable tokens.
    if (scanner_match_symbols(lexer))     return;
    if (scanner_match_numbers(lexer))     return;
    if (scanner_match_strings(lexer))     return;
    if (scanner_match_identifiers(lexer)) return;

    // If we reach here, we have an error token. Consume 1 and return it.
    char c;
    scanner_consume(lexer, &c);
    *lexer->next_token = scanner_create_token(lexer, tokentype::TOKEN_ERROR_UNKNOWN);
    scanner_synchronize(lexer);

    return;

}

static inline token
scanner_create_token(scanner *lexer, tokentype type)
{

    string str;
    for (u64 i = lexer->source_offset; i < lexer->source_step; i++)
    {
        char c = lexer->source_string[i];
        str += c;
    }

    i32 column = lexer->source_column - str.length();
    i32 row = lexer->source_line;

    // Uh oh, we need to manually calculate the rows and columns!
    if (column < 0)
    {

        column = 1;
        row = 1;

        for (u64 i = 0; i < lexer->source_offset; i++)
        {

            char c = lexer->source_string[i];
            if (c == '\n')
            {
                row++;
                column = 1;
            }
            else
            {
                column++;
            }

        }

    }

    return token(str,
        lexer->source_file_path,
        type, 
        row,
        column);

}
