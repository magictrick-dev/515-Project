#ifndef SOURCE_COMPILER_LEXER_TOKENIZER_HPP
#define SOURCE_COMPILER_LEXER_TOKENIZER_HPP
#include <definitions.hpp>
#include <utilities/buffer.hpp>
#include <compiler/lexer/token.hpp>

class Tokenizer
{

    public:
                        Tokenizer();
        virtual        ~Tokenizer();

        bool            initialize(ccptr file_path);

        void            shift();

        Token           get_previous_token() const;
        Token           get_current_token() const;
        Token           get_next_token() const;

        bool            previous_token_is(TokenType type) const;
        bool            current_token_is(TokenType type) const;
        bool            next_token_is(TokenType type) const;
        bool            previous_token_is(TokenType type, string reference) const;
        bool            current_token_is(TokenType type, string reference) const;
        bool            next_token_is(TokenType type, string reference) const;
        
        TokenType       get_previous_token_type() const;
        TokenType       get_current_token_type() const;
        TokenType       get_next_token_type() const;

    protected:
        bool            advance();
        bool            consume(char *c);
        bool            examine(char *c);
        bool            peek(char *c);
        bool            peek_ahead(u64 step, char *c);

        void            synchronize();

        template <typename ...Args> bool match(Args... args);

        bool            consume_whitespace();

        bool            match_symbols();
        bool            match_numbers();
        bool            match_strings();
        bool            match_identifiers();
        void            match_token();

        Token           create_token(TokenType type);

    protected:
        memory_buffer   source_buffer;
        string          source_path;
        cptr            source_string;
        u64             source_size;
        u64             source_offset;
        u64             source_step;
        u64             source_line;
        u64             source_column;

        Token   tokens[3];
        Token*  previous_token;
        Token*  current_token;
        Token*  next_token;

        string  last_error;

};

#endif
