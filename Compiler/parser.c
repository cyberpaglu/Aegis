// parser.c
#include <stdio.h>
#include <string.h>
#include "parser.h"

static void expect(Token *tok, TokenType type, const char *name) {
    if (tok->type != type) {
        fprintf(stderr, "Parse error: expected %s, got '%s'\n", name, tok->text);
    }
}

int parse_rule(Lexer *lexer, AST *out_ast) {
    Token tok;

    // 1. if
    tok = next_token(lexer);
    expect(&tok, TOKEN_IF, "IF");
    if (tok.type != TOKEN_IF) return 1;

    // 2. condition (IDENT)
    tok = next_token(lexer);
    expect(&tok, TOKEN_IDENT, "IDENT (condition)");
    if (tok.type != TOKEN_IDENT) return 1;
    strncpy(out_ast->condition, tok.text, sizeof(out_ast->condition));

    // 3. then
    tok = next_token(lexer);
    expect(&tok, TOKEN_THEN, "THEN");
    if (tok.type != TOKEN_THEN) return 1;

    // 4. action IDENT
    tok = next_token(lexer);
    expect(&tok, TOKEN_IDENT, "IDENT (action)");
    if (tok.type != TOKEN_IDENT) return 1;
    strncpy(out_ast->action, tok.text, sizeof(out_ast->action));

    // 5. '('
    tok = next_token(lexer);
    expect(&tok, TOKEN_LPAREN, "(");
    if (tok.type != TOKEN_LPAREN) return 1;

    // 6. string
    tok = next_token(lexer);
    expect(&tok, TOKEN_STRING, "STRING");
    if (tok.type != TOKEN_STRING) return 1;
    strncpy(out_ast->argument, tok.text, sizeof(out_ast->argument));

    // 7. ')'
    tok = next_token(lexer);
    expect(&tok, TOKEN_RPAREN, ")");
    if (tok.type != TOKEN_RPAREN) return 1;

    return 0; // success
}

