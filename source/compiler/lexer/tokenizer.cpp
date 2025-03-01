#include "platform/system.hpp"
#include <compiler/lexer/tokenizer.hpp>
#include <platform/filesystem.hpp>

// --- Public ------------------------------------------------------------------

Tokenizer::
Tokenizer()
{
    
}

Tokenizer::
~Tokenizer()
{
    
}

bool Tokenizer::
initialize(ccptr file_path)
{

    if (!filesystem_file_exists(file_path))
        return false;

    u64 file_size;
    if (!filesystem_get_file_size(file_path, &file_size))
        return false;

    this->source_path       = file_path;
    this->source_buffer     = system_virtual_allocate(nullptr, file_size+1);
    this->source_size       = file_size;
    this->source_string     = (char*)(this->source_buffer.data);
    this->source_offset     = 0;
    this->source_step       = 0;
    this->source_column     = 1;
    this->source_line       = 1;

    if (!filesystem_read_entire_file(file_path, &this->source_buffer))
        system_virtual_free(this->source_buffer.data, this->source_buffer.size);
    ((char*)this->source_buffer.data)[file_size] = '\0';

    this->previous_token    = &this->tokens[0];
    this->current_token     = &this->tokens[1];
    this->next_token        = &this->tokens[2];
    this->last_error        = "";

    this->shift();
    this->shift();
    
    return true;
    
}

void Tokenizer::
shift()
{

    Token *placeholder      = this->previous_token;
    this->previous_token    = this->current_token;
    this->current_token     = this->next_token;
    this->next_token        = placeholder;

    this->match_token();

}

Token Tokenizer::
get_previous_token() const
{

    return *this->previous_token;

}

Token Tokenizer::
get_current_token() const
{

    return *this->current_token;

}

TokenType Tokenizer::
get_previous_token_type() const
{

    return this->previous_token->get_type();

}

TokenType Tokenizer::
get_current_token_type() const
{

    return this->current_token->get_type();

}

TokenType Tokenizer::
get_next_token_type() const
{

    return this->next_token->get_type();

}

Token Tokenizer::
get_next_token() const
{

    return *this->next_token;

}

bool Tokenizer::
previous_token_is(TokenType type) const
{

    if (this->previous_token->get_type() == type)
        return true;
    return false;

}

bool Tokenizer::
current_token_is(TokenType type) const
{

    if (this->current_token->get_type() == type)
        return true;
    return false;

}

bool Tokenizer::
next_token_is(TokenType type) const
{

    if (this->next_token->get_type() == type)
        return true;
    return false;

}

bool Tokenizer::
previous_token_is(TokenType type, string reference) const
{

    if (this->previous_token->get_type() == type &&
        this->previous_token->get_reference() == reference)
        return true;
    return false;

}

bool Tokenizer::
current_token_is(TokenType type, string reference) const
{

    if (this->current_token->get_type() == type &&
        this->current_token->get_reference() == reference)
        return true;
    return false;

}

bool Tokenizer::
next_token_is(TokenType type, string reference) const
{

    if (this->next_token->get_type() == type &&
        this->next_token->get_reference() == reference)
        return true;
    return false;

}

// --- Internal ----------------------------------------------------------------

bool Tokenizer::
advance()
{

    if (this->source_step >= this->source_size)
        return false;

    char c = this->source_string[this->source_step];
    if (c == '\n')
    {
        this->source_line++;
        this->source_column = 1;
    }
    else
    {
        this->source_column++;
    }

    this->source_step++;
    return true;

}

bool Tokenizer::
consume(char *c)
{

    if (this->source_step >= this->source_size)
        return false;

    *c = this->source_string[this->source_step];
    if (*c == '\n')
    {
        this->source_line++;
        this->source_column = 1;
    }
    else
    {
        this->source_column++;
    }

    this->source_step++;
    return true;

}

bool Tokenizer::
examine(char *c)
{

    if (this->source_step >= this->source_size)
        return false;

    *c = this->source_string[this->source_step];
    return true;

}

bool Tokenizer::
peek(char *c)
{

    if (this->source_step+1 >= this->source_size)
        return false;

    *c = this->source_string[this->source_step+1];
    return true;

}

bool Tokenizer::
peek_ahead(u64 step, char *c)
{

    if (this->source_step+step >= this->source_size)
        return false;

    *c = this->source_string[this->source_step+step];
    return true;

}

template <typename ...Args> bool Tokenizer::
match(Args... args)
{

    char current_character;
    if (!this->examine(&current_character))
        return false;

    for (auto c : { args... })
    {
        if (current_character == c)
            return true;
    }

    return false;

}

void Tokenizer::
synchronize()
{

    this->source_offset = this->source_step;

}

bool Tokenizer::
consume_whitespace()
{

    // White space characters.
    if (this->match(' ', '\n', '\r', '\t'))
    {
        this->advance();
        return true;
    }

    // Single line comments.
    if (this->match('#'))
    {

        this->advance();

        while (true)
        {

            char c;
            if (!this->consume(&c))
            {
                this->last_error = "Unexpected EOF.";
                return false;
            }

            if (c == '\n')
                break;

        }

        return true;

    }

    // Block comments begin with <<- and end with ->>.
    if (this->match('<'))
    {

        char head;
        char body;
        char tail;

        this->examine(&head);
        this->peek(&body);
        this->peek_ahead(2, &tail);

        if (head == '<' && body == '<' && tail == '-')
        {

            this->advance();
            this->advance();
            this->advance();

            while (true)
            {

                if (!this->consume(&head))
                {
                    this->last_error = "Unexpected EOF.";
                    return false;
                }

                if (!this->examine(&body))
                {
                    this->last_error = "Unexpected EOF.";
                    return false;
                }

                if (!this->peek(&tail))
                {
                    this->last_error = "Unexpected EOF.";
                    return false;
                }

                if (head == '-' && body == '>' && tail == '>')
                {
                    this->advance();
                    this->advance();
                    this->advance();
                    break;
                }

            }

            return true;

        }

    }

    return false;

}

bool Tokenizer::
match_symbols()
{

    char c;
    if (!this->examine(&c)) return false; // EOF.

    switch (c)
    {
        case '+':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_PLUS);
            this->synchronize();
            return true;

        } break;

        case '-':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_MINUS);
            this->synchronize();
            return true;

        } break;

        case '*':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_MULTIPLY);
            this->synchronize();
            return true;

        } break;

        case '/':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_DIVIDE);
            this->synchronize();
            return true;

        } break;

        case '^':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_EXPONENT);
            this->synchronize();
            return true;

        } break;

        case '<':
        {

            char peek;
            this->peek(&peek);

            if (peek == '=')
            {
                this->consume(&c);
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_LESS_EQUAL);
                this->synchronize();
                return true;
            }

            else if (peek == '-')
            {
                this->consume(&c);
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_ASSIGN);
                this->synchronize();
                return true;
            }

            else
            {
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_LESS);
                this->synchronize();
                return true;
            }

        } break;

        case '>':
        {

            char peek;
            this->peek(&peek);

            if (peek == '=')
            {
                this->consume(&c);
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_GREATER_EQUAL);
                this->synchronize();
                return true;
            }
            else
            {
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_GREATER);
                this->synchronize();
                return true;
            }

        } break;

        case '=':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_EQUAL);
            this->synchronize();
            return true;

        } break;

        case '~':
        {

            // If we can't find a token, then this token is invalid.
            char peek;
            if (!this->peek(&peek))
                return false;

            // Must be an equals.
            if (peek == '=')
            {
                this->consume(&c);
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_NOT_EQUAL);
                this->synchronize();
                return true;
            }

            else
            {
                return false;
            }

        } break;

        case '!':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_NOT);
            this->synchronize();
            return true;

        } break;

        case '(':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_LEFT_PARENTHESES);
            this->synchronize();
            return true;

        } break;

        case ')':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_RIGHT_PARENTHESES);
            this->synchronize();
            return true;

        } break;

        case '[':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_LEFT_BRACKET);
            this->synchronize();
            return true;

        } break;

        case ']':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_RIGHT_BRACKET);
            this->synchronize();
            return true;

        } break;

        case '{':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_LEFT_BRACE);
            this->synchronize();
            return true;

        } break;

        case '}':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_RIGHT_BRACE);
            this->synchronize();
            return true;

        } break;

        case '&':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_AND);
            this->synchronize();
            return true;

        } break;

        case '|':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_OR);
            this->synchronize();
            return true;

        } break;

        case '.':
        {

            char peek;
            this->peek(&peek);

            // Handled by numbers.
            if (isdigit(peek))
            {
                return false;
            }

            // Otherwise, just a dot.
            else
            {
                this->consume(&c);
                *this->next_token = this->create_token(TokenType::TOKEN_DOT);
                this->synchronize();
                return true;
            }

        } break;

        case '@':
        {

            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_AT);
            this->synchronize();
            return true;

        } break;

        case ':':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_COLON);
            this->synchronize();
            return true;

        } break;

        case ';':
        {

            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_SEMICOLON);
            this->synchronize();
            return true;

        }

        case ',':
        {
            
            this->consume(&c);
            *this->next_token = this->create_token(TokenType::TOKEN_COMMA);
            this->synchronize();
            return true;

        } break;

    }

    return false;


}

bool Tokenizer::
match_numbers()
{

    char initial_character;
    if (!this->examine(&initial_character)) return false; // EOF.

    // Must start with a number or a decimal.
    if (isdigit(initial_character) || initial_character == '.')
    {

        // Base case.
        bool exponentiated = false;
        TokenType type = TokenType::TOKEN_INTEGER;
        if (initial_character == '.') type = TokenType::TOKEN_REAL;

        this->advance();

        char check;
        while (true)
        {

            if (!this->examine(&check))
                break; // EOF.

            if (isdigit(check))
            {
                this->advance();
            }

            else if (check == '.')
            {

                // If we already encountered a decimal, break.
                if (type == TokenType::TOKEN_REAL)
                    break;

                // If the following character is an e/E it is not valid.
                char peek;
                if (!this->peek(&peek))
                {
                    type = TokenType::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

                if (peek == 'e' || peek == 'E')
                {
                    type = TokenType::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

                if (type == TokenType::TOKEN_INTEGER)
                    type = TokenType::TOKEN_REAL;
                this->advance();

            }

            else if (check == 'e' || check == 'E')
            {

                // The following character must either be a number or +/-.
                char peek;
                if (!this->peek(&peek))
                {
                    type = TokenType::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

                if (isdigit(peek))
                {
                    if (type == TokenType::TOKEN_INTEGER)
                        type = TokenType::TOKEN_REAL;
                    this->advance();
                    this->advance();
                    exponentiated = true;
                }

                else if (peek == '+' || peek == '-')
                {

                    // The following character must be a number.
                    char peek_ahead;
                    if (!this->peek_ahead(2, &peek_ahead))
                    {
                        type = TokenType::TOKEN_ERROR_INVALID_REAL;
                        break;
                    }

                    if (isdigit(peek_ahead))
                    {
                        if (type == TokenType::TOKEN_INTEGER)
                            type = TokenType::TOKEN_REAL;
                        this->advance();
                        this->advance();
                        this->advance();
                        exponentiated = true;
                    }

                    else
                    {
                        type = TokenType::TOKEN_ERROR_INVALID_REAL;
                        break;
                    }

                }

                else
                {
                    type = TokenType::TOKEN_ERROR_INVALID_REAL;
                    break;
                }

            }

            else
            {
                break;
            }

        }

        // Integers can't start with zeros.
        if (type == TokenType::TOKEN_INTEGER && initial_character == '0')
            type = TokenType::TOKEN_ERROR_INVALID_INTEGER;

        *this->next_token = this->create_token(type);
        this->synchronize();
        return true;

    }

    return false;

}

bool Tokenizer::
match_strings()
{

    char c;
    if (!this->examine(&c)) return false; // EOF.

    // Must start with a double quote.
    if (c == '"')
    {

        TokenType type = TokenType::TOKEN_STRING;
        this->advance();

        while (true)
        {

            char check;
            if (!this->examine(&check))
            {
                *this->next_token = this->create_token(TokenType::TOKEN_ERROR_UNEXPECTED_EOF);
                this->synchronize();
                return true;
            }

            if (check == '\\')
            {
                char peek;
                if (!this->peek(&peek))
                {
                    *this->next_token = this->create_token(TokenType::TOKEN_ERROR_UNEXPECTED_EOF);
                    this->synchronize();
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
                            if (!this->peek_ahead(i+2, &unicode[i]))
                            {
                                *this->next_token = this->create_token(TokenType::TOKEN_ERROR_INVALID_ESCAPE);
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
                            this->advance();
                            this->advance();
                            this->advance();
                            this->advance();
                            this->advance();
                            this->advance();

                        }

                        else
                        {
                            type = TokenType::TOKEN_ERROR_INVALID_ESCAPE;
                        }

                    } break;

                    default:
                    {
                        type = TokenType::TOKEN_ERROR_INVALID_ESCAPE;
                    } break;
                }

                this->advance();
                this->advance();

            }

            else if (check == '\"')
            {
                this->advance();
                break;
            }

            else
            {
                this->advance();
            }

        }

        *this->next_token = this->create_token(type);
        this->synchronize();
        return true;

    }

    return false;

}

bool Tokenizer::
match_identifiers()
{

    char c;
    if (!this->examine(&c)) return false; // EOF.

    // Must start with a letter, lowercase or uppercase is fine.
    if (isalpha(c) || c == '_')
    {

        this->advance();

        // Basically everything else can be a combination of letters and numbers.
        while (true)
        {

            char check;
            if (!this->examine(&check))
                break; // EOF.

            if (isalnum(check) || check == '_')
            {
                this->advance();
            }

            else
            {
                break;
            }

        }

        *this->next_token = this->create_token(TokenType::TOKEN_IDENTIFIER);
        this->synchronize();
        return true;

    }

    return false;

}

void Tokenizer::
match_token()
{
    
    // Consume all whitespace.
    while (true)
    {

        bool consume_results = this->consume_whitespace();
        if (consume_results)
        {
            this->synchronize();
            continue;
        }
        else if (this->last_error != "")
        {
            *this->next_token = this->create_token(
                TokenType::TOKEN_ERROR_UNEXPECTED_EOF);
            this->synchronize();
            this->last_error = ""; // Reset last error.
            return;
        }
        else
        {
            this->synchronize();
            break;
        }

    }

    if (this->last_error != "")
    {
        std::cout << this->last_error << std::endl;
        this->last_error = ""; // Reset the error.
    }

    // If we are at the end, just return an EOF token.
    if (this->source_step >= this->source_size)
    {
        *this->next_token = this->create_token(TokenType::TOKEN_EOF);
        this->synchronize();
        return;
    }

    // Process all viable tokens.
    if (this->match_symbols())     return;
    if (this->match_numbers())     return;
    if (this->match_strings())     return;
    if (this->match_identifiers()) return;

    // If we reach here, we have an error token. Consume 1 and return it.
    char c;
    this->consume(&c);
    *this->next_token = this->create_token(TokenType::TOKEN_ERROR_UNKNOWN);
    this->synchronize();

    return;

}

Token Tokenizer::
create_token(TokenType type)
{

    string str;
    for (u64 i = this->source_offset; i < this->source_step; i++)
    {
        char c = this->source_string[i];
        str += c;
    }

    i32 column = this->source_column - str.length();
    i32 row = this->source_line;

    // Uh oh, we need to manually calculate the rows and columns!
    if (column < 0)
    {

        column = 1;
        row = 1;

        for (u64 i = 0; i < this->source_offset; i++)
        {

            char c = this->source_string[i];
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

    return Token(str, this->source_path, type, row, column);

}
