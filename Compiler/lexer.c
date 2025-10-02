// lexer.c
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

void init_lexer(Lexer *l, const char *input) {
    l->src = input;
    l->pos = 0;
}

static char peek(Lexer *l) {
    return l->src[l->pos];
}

static char advance(Lexer *l) {
    return l->src[l->pos++];
}

static void skip_whitespace(Lexer *l) {
    while (isspace(peek(l))) advance(l);
}

static void read_identifier(Lexer *l, Token *t) {
    size_t start = l->pos;
    while (isalnum(peek(l)) || peek(l)=='.' || peek(l)=='_') advance(l);
    size_t len = l->pos - start;
    strncpy(t->text, l->src + start, len);
    t->text[len] = '\0';

    if (strcmp(t->text,"if")==0) t->type = TOKEN_IF;
    else if (strcmp(t->text,"then")==0) t->type = TOKEN_THEN;
    else t->type = TOKEN_IDENT;
}

static void read_string(Lexer *l, Token *t) {
    advance(l); // skip opening quote
    size_t start = l->pos;
    while (peek(l) != '"' && peek(l) != '\0') advance(l);
    size_t len = l->pos - start;
    strncpy(t->text, l->src + start, len);
    t->text[len] = '\0';
    if (peek(l) == '"') advance(l); // skip closing quote
    t->type = TOKEN_STRING;
}

Token next_token(Lexer *l) {
    skip_whitespace(l);
    Token t;
    t.type = TOKEN_UNKNOWN;
    t.text[0] = '\0';

    char c = peek(l);
    if (c == '\0') { t.type = TOKEN_EOF; return t; }

    if (isalpha(c)) { read_identifier(l, &t); return t; }
    if (c == '"')   { read_string(l, &t);    return t; }

    // Single-character tokens
    if (c == '(') { advance(l); t.type = TOKEN_LPAREN; strcpy(t.text,"("); return t; }
    if (c == ')') { advance(l); t.type = TOKEN_RPAREN; strcpy(t.text,")"); return t; }

    // If we reach here, unknown char
    advance(l);
    t.type = TOKEN_UNKNOWN;
    snprintf(t.text, sizeof(t.text), "%c", c);
    return t;
}

