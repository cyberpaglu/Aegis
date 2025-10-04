// lexer.h
#ifndef LEXER_H
#define LEXER_H

typedef enum {
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_SEMI,
    TOKEN_IDENT,
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_STRING,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    int value;         // used if it's a number
    char text[64];     // used if it's an identifier/string
} Token;

typedef struct {
    const char *src;
    int pos;
} Lexer;

void init_lexer(Lexer *l, const char *src);
Token next_token(Lexer *l);

#endif
