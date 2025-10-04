#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

void init_lexer(Lexer *l, const char *src) {
    l->src = src;
    l->pos = 0;
}

static char peek(Lexer *l) {
    return l->src[l->pos];
}

static char advance(Lexer *l) {
    return l->src[l->pos++] ;
}

static void skip_whitespace(Lexer *l) {
    while (isspace(peek(l))) advance(l);
}

static Token read_number(Lexer *l) {
    Token t;
    t.type = TOKEN_NUMBER;
    t.value = 0;
    while (isdigit(peek(l))) {
        t.value = t.value * 10 + (advance(l) - '0');
    }
    snprintf(t.text, sizeof(t.text), "%d", t.value);
    return t;
}

static Token read_identifier(Lexer *l) {
    Token t;
    int start = l->pos;
    while (isalnum(peek(l)) || peek(l) == '_') advance(l);
    int len = l->pos - start;
    strncpy(t.text, l->src + start, len);
    t.text[len] = '\0';

    if (strcmp(t.text,"if")==0) t.type = TOKEN_IF;
    else if (strcmp(t.text,"then")==0) t.type = TOKEN_THEN;
    else t.type = TOKEN_IDENT;
    return t;
}

static Token read_string(Lexer *l) {
    Token t;
    int start = ++l->pos; // skip opening "
    while (peek(l) != '"' && peek(l) != '\0') advance(l);
    int len = l->pos - start;
    strncpy(t.text, l->src + start, len);
    t.text[len] = '\0';
    if (peek(l) == '"') advance(l); // skip closing "
    t.type = TOKEN_STRING;
    return t;
}

Token next_token(Lexer *l) {
    skip_whitespace(l);
    Token t;
    t.type = TOKEN_UNKNOWN;
    t.text[0] = '\0';
    t.value = 0;

    char c = peek(l);
    if (c == '\0') {
        t.type = TOKEN_EOF;
        return t;
    }

    if (isdigit(c)) return read_number(l);
    if (isalpha(c) || c == '_') return read_identifier(l);
    if (c == '"') return read_string(l);

    switch (c) {
        case '+': advance(l); t.type = TOKEN_PLUS; strcpy(t.text, "+"); return t;
        case '-': advance(l); t.type = TOKEN_MINUS; strcpy(t.text, "-"); return t;
        case '*': advance(l); t.type = TOKEN_STAR; strcpy(t.text, "*"); return t;
        case '/': advance(l); t.type = TOKEN_SLASH; strcpy(t.text, "/"); return t;
        case '(': advance(l); t.type = TOKEN_LPAREN; strcpy(t.text, "("); return t;
        case ')': advance(l); t.type = TOKEN_RPAREN; strcpy(t.text, ")"); return t;
        case ';': advance(l); t.type = TOKEN_SEMI; strcpy(t.text, ";"); return t;
        default:
            snprintf(t.text, sizeof(t.text), "%c", advance(l));
            t.type = TOKEN_UNKNOWN;
            return t;
    }
}
