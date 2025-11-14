// parser.h
#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

// --- Redefined AST Node Types for the IDS Language ---
typedef enum {
    NODE_PROGRAM,       // Represents the whole program (a list of statements)
    NODE_IF_STATEMENT,  // Represents an 'if condition then action' rule
    NODE_CONDITION,     // Represents the condition part (e.g., 'tcp.syn')
    NODE_ACTION         // Represents the action part (e.g., 'alert("...")')
} NodeType;

typedef struct ASTNode {
    NodeType type;
    struct ASTNode *next; // Pointer to the next statement in a program
    union {
        // For NODE_IF_STATEMENT
        struct {
            struct ASTNode *condition;
            struct ASTNode *action;
        } if_stmt;

        // For NODE_CONDITION
        struct {
            char identifier[64];
        } condition;

        // For NODE_ACTION
        struct {
            char message[64];
        } action;

        // For NODE_PROGRAM
        struct {
            struct ASTNode *statements;
        } program;
    };
} ASTNode;


typedef struct {
    Lexer *lexer;
    Token current;
} Parser;

void init_parser(Parser *p, Lexer *l);
ASTNode *parse_program(Parser *p);
void print_ast(ASTNode *node, int indent);
void free_ast(ASTNode *node);

#endif
