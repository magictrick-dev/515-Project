#include <compiler/lexer/token.hpp>
#include <utilities/intrinsics.hpp>

Token::
Token()
{
    this->reference = "";
    this->source    = "";
    this->type      = TokenType::TOKEN_NULL;
    this->line      = 0;
    this->column    = 0;
}

Token::
Token(string reference, string source, TokenType type, i32 line, i32 column)
{
    this->reference = reference;
    this->source    = source;
    this->type      = type;
    this->line      = line;
    this->column    = column;
}

Token::
~Token()
{
    this->reference = "";
    this->source    = "";
    this->type      = TokenType::TOKEN_NULL;
    this->line      = 0;
    this->column    = 0;
}

string Token::
get_reference() const
{
    return this->reference;
}

string Token::
get_source() const
{
    return this->source;
}

TokenType Token::
get_type() const
{
    return this->type;
}

string Token::
get_type_string() const
{

    switch (this->type)
    {

        case TokenType::TOKEN_NULL:                     return "TOKEN_NULL";
        case TokenType::TOKEN_ERROR_UNKNOWN:            return "TOKEN_ERROR_UNKNOWN";
        case TokenType::TOKEN_ERROR_INVALID_INTEGER:    return "TOKEN_ERROR_INVALID_INTEGER";
        case TokenType::TOKEN_ERROR_INVALID_REAL:       return "TOKEN_ERROR_INVALID_REAL";
        case TokenType::TOKEN_ERROR_UNEXPECTED_EOF:     return "TOKEN_ERROR_UNEXPECTED_EOF";
        case TokenType::TOKEN_ERROR_INVALID_ESCAPE:     return "TOKEN_ERROR_INVALID_ESCAPE";

        case TokenType::TOKEN_EOF:                 return "TOKEN_EOF";
        case TokenType::TOKEN_PLUS:                return "TOKEN_PLUS";
        case TokenType::TOKEN_MINUS:               return "TOKEN_MINUS";
        case TokenType::TOKEN_MULTIPLY:            return "TOKEN_MULTIPLY";
        case TokenType::TOKEN_DIVIDE:              return "TOKEN_DIVIDE";
        case TokenType::TOKEN_IDENTIFIER:          return "TOKEN_IDENTIFIER";
        case TokenType::TOKEN_EXPONENT:            return "TOKEN_EXPONENT";
        case TokenType::TOKEN_LESS:                return "TOKEN_LESS";
        case TokenType::TOKEN_LESS_EQUAL:          return "TOKEN_LESS_EQUAL";
        case TokenType::TOKEN_GREATER:             return "TOKEN_GREATER";
        case TokenType::TOKEN_GREATER_EQUAL:       return "TOKEN_GREATER_EQUAL";
        case TokenType::TOKEN_EQUAL:               return "TOKEN_EQUAL";
        case TokenType::TOKEN_NOT_EQUAL:           return "TOKEN_NOT_EQUAL";
        case TokenType::TOKEN_ASSIGN:              return "TOKEN_ASSIGN";
        case TokenType::TOKEN_NOT:                 return "TOKEN_NOT";
        case TokenType::TOKEN_LEFT_PARENTHESES:    return "TOKEN_LEFT_PARENTHESES";
        case TokenType::TOKEN_RIGHT_PARENTHESES:   return "TOKEN_RIGHT_PARENTHESES";
        case TokenType::TOKEN_LEFT_BRACKET:        return "TOKEN_LEFT_BRACKET";
        case TokenType::TOKEN_RIGHT_BRACKET:       return "TOKEN_RIGHT_BRACKET";
        case TokenType::TOKEN_LEFT_BRACE:          return "TOKEN_LEFT_BRACE";
        case TokenType::TOKEN_RIGHT_BRACE:         return "TOKEN_RIGHT_BRACE";
        case TokenType::TOKEN_AND:                 return "TOKEN_AND";
        case TokenType::TOKEN_OR:                  return "TOKEN_OR";
        case TokenType::TOKEN_DOT:                 return "TOKEN_DOT";
        case TokenType::TOKEN_AT:                  return "TOKEN_AT";
        case TokenType::TOKEN_INTEGER:             return "TOKEN_INTEGER";
        case TokenType::TOKEN_STRING:              return "TOKEN_STRING";
        case TokenType::TOKEN_COLON:               return "TOKEN_COLON";
        case TokenType::TOKEN_SEMICOLON:           return "TOKEN_SEMICOLON";
        case TokenType::TOKEN_COMMA:               return "TOKEN_COMMA";
        case TokenType::TOKEN_REAL:                return "TOKEN_REAL";
        default: NOREACH("Unknown token type.");

    }

    return ""; // Otherwise the compiler will throw a fit.

}

i32 Token::
get_line() const
{
    return this->line;
}

i32 Token::
get_column() const
{
    return this->column;
}

string Token::
format() const
{

    string result = this->source;
    result += "(";
    result += std::to_string(this->line);
    result += ",";
    result += std::to_string(this->column);
    result += "): ";
    result += this->get_type_string();
    result += " ";
    if (this->type != TokenType::TOKEN_STRING)
        result += this->reference;
    else
        result += this->parse_reference_as_string();

    return result;

}

string Token::
parse_reference_as_string() const
{

    std::string result = "";
    u64 length = this->reference.length();
    for (u64 i = 0; i < length; i++)
    {
        if (this->reference[i] == '\\')
        {
            if (i + 1 < length)
            {
                switch (this->reference[i + 1])
                {
                    case 'n': result += '\n'; break;
                    case 'r': result += '\r'; break;
                    case 't': result += '\t'; break;
                    case 'a': result += '\a'; break;
                    case 'b': result += '\b'; break;
                    case '\\': result += '\\'; break;
                    case '\"': result += '\"'; break;
                    case 'u':
                    {
                        
                        // Unicode escape. Check ahead for 6 characters.
                        char unicode[7];
                        unicode[6] = '\0';
                        for (u64 j = 0; j < 6; j++)
                        {
                            if (i + j + 2 < length)
                            {
                                unicode[j] = this->reference[i + j + 2];
                            }
                            else
                            {
                                return result.c_str();
                            }
                        }

                        // Check if all characters are valid hex.
                        bool valid = true;
                        for (u64 j = 0; j < 6; j++)
                        {
                            if (!isxdigit(unicode[j]))
                            {
                                valid = false;
                                break;
                            }
                        }

                        if (!valid)
                        {
                            result += this->reference[i];
                        }

                        // Convert the hex string to a number.
                        u32 codepoint = 0;
                        //sscanf(unicode, "%x", &codepoint);
                        //sscanf(unicode, "%x", &codepoint);
                        intrinsic_sscanf(unicode, "%x", &codepoint);

                        // Convert the codepoint to a utf8 character.
                        char utf_string[8];
                        intrinsic_codepoint_to_utf8(codepoint, utf_string);
                        result += utf_string;
                        i+=6;

                    } break;
                    default: result += this->reference[i]; break;
                }
                i++;
            }
            else
            {
                result += this->reference[i];
            }
        }
        else
        {
            result += this->reference[i];
        }
    }

    return result.c_str();

}
