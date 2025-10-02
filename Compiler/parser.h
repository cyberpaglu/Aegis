// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef struct {
    char condition[128];
    char action[64];
    char argument[128];
} AST; // Very simple AST for one rule

// Parse the tokens into an AST
// Returns 0 if success, non-zero on error
int parse_rule(Lexer *lexer, AST *out_ast);

#endif

