// lexer.h
#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>  // for size_t

typedef enum {
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_IDENT,
    TOKEN_STRING,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char text[128];   // store raw text (identifier or string)
} Token;

/*  ---- ADD THESE ----  */
typedef struct {
    const char *src;
    size_t pos;
} Lexer;

void init_lexer(Lexer *l, const char *input);
Token next_token(Lexer *l);

#endif

