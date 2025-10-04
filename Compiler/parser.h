#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_NUMBER,
    NODE_BINARY
} NodeType;

typedef struct ASTNode {
    NodeType type;
    union {
        int value; // For numbers
        struct {
            struct ASTNode *left;
            TokenType op;   // TOK_PLUS, TOK_MINUS, etc.
            struct ASTNode *right;
        } binary;
    };
} ASTNode;

typedef struct {
    Lexer *lexer;
    Token current;
} Parser;

void init_parser(Parser *parser, Lexer *lexer);
ASTNode *parse_program(Parser *parser);
void free_ast(ASTNode *node);
void print_ast(ASTNode *node, int indent);

#endif
