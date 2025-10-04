#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

ASTNode *parse_expression(Parser *parser);
ASTNode *parse_term(Parser *parser);
ASTNode *parse_factor(Parser *parser);

void advance(Parser *parser) {
    parser->current = next_token(parser->lexer);
}

void init_parser(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    advance(parser);
}

ASTNode *make_number(int value) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_NUMBER;
    node->value = value;
    return node;
}

ASTNode *make_binary(ASTNode *left, TokenType op, ASTNode *right) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY;
    node->binary.left = left;
    node->binary.op = op;
    node->binary.right = right;
    return node;
}

ASTNode *parse_factor(Parser *parser) {
    if (parser->current.type == TOKEN_NUMBER) {
        ASTNode *node = make_number(parser->current.value);
        advance(parser);
        return node;
    } else if (parser->current.type == TOKEN_LPAREN) {
        advance(parser);
        ASTNode *node = parse_expression(parser);
        if (parser->current.type != TOKEN_RPAREN) {
            printf("Error: expected ')'\n");
            exit(1);
        }
        advance(parser);
        return node;
    }
    printf("Error: unexpected token in factor\n");
    exit(1);
}

ASTNode *parse_term(Parser *parser) {
    ASTNode *node = parse_factor(parser);
    while (parser->current.type == TOKEN_STAR || parser->current.type == TOKEN_SLASH) {
        TokenType op = parser->current.type;
        advance(parser);
        ASTNode *right = parse_factor(parser);
        node = make_binary(node, op, right);
    }
    return node;
}

ASTNode *parse_expression(Parser *parser) {
    ASTNode *node = parse_term(parser);
    while (parser->current.type == TOKEN_PLUS || parser->current.type == TOKEN_MINUS) {
        TokenType op = parser->current.type;
        advance(parser);
        ASTNode *right = parse_term(parser);
        node = make_binary(node, op, right);
    }
    return node;
}

ASTNode *parse_statement(Parser *parser) {
    ASTNode *expr = parse_expression(parser);
    if (parser->current.type != TOKEN_SEMI) {
        printf("Error: expected ';'\n");
        exit(1);
    }
    advance(parser);
    return expr;
}

ASTNode *parse_program(Parser *parser) {
    return parse_statement(parser);
}

void print_ast(ASTNode *node, int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
    if (node->type == NODE_NUMBER) {
        printf("Number(%d)\n", node->value);
    } else if (node->type == NODE_BINARY) {
        printf("Binary(");
        switch (node->binary.op) {
            case TOKEN_PLUS: printf("+"); break;
            case TOKEN_MINUS: printf("-"); break;
            case TOKEN_STAR: printf("*"); break;
            case TOKEN_SLASH: printf("/"); break;
            default: printf("?"); break;
        }
        printf(")\n");
        print_ast(node->binary.left, indent + 1);
        print_ast(node->binary.right, indent + 1);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    if (node->type == NODE_BINARY) {
        free_ast(node->binary.left);
        free_ast(node->binary.right);
    }
    free(node);
}
