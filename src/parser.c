#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// Forward declarations
ASTNode *parse_statement(Parser *parser);

void advance(Parser *parser) {
    parser->current = next_token(parser->lexer);
}

void init_parser(Parser *parser, Lexer *lexer) {
    parser->lexer = lexer;
    advance(parser);
}

// --- AST Node Creation Functions ---

ASTNode *make_node(NodeType type) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if (!node) {
        perror("Failed to allocate AST node");
        exit(1);
    }
    node->type = type;
    node->next = NULL;
    return node;
}

ASTNode *make_if_statement(ASTNode *condition, ASTNode *action) {
    ASTNode *node = make_node(NODE_IF_STATEMENT);
    node->if_stmt.condition = condition;
    node->if_stmt.action = action;
    return node;
}

ASTNode *make_condition(const char *identifier) {
    ASTNode *node = make_node(NODE_CONDITION);
    strncpy(node->condition.identifier, identifier, sizeof(node->condition.identifier) - 1);
    node->condition.identifier[sizeof(node->condition.identifier) - 1] = '\0';
    return node;
}

ASTNode *make_action(const char *message) {
    ASTNode *node = make_node(NODE_ACTION);
    strncpy(node->action.message, message, sizeof(node->action.message) - 1);
    node->action.message[sizeof(node->action.message) - 1] = '\0';
    return node;
}

// --- Parser Logic ---

// Parses an action, e.g., alert("message")
// Note: This is a simplified version. A real one would need to parse different action types.
ASTNode *parse_action(Parser *parser) {
    if (parser->current.type != TOKEN_ALERT) {
        printf("Error: Expected 'alert' keyword.\n");
        exit(1);
    }
    advance(parser); // Consume 'alert'

    if (parser->current.type != TOKEN_LPAREN) {
        printf("Error: Expected '(' after 'alert'.\n");
        exit(1);
    }
    advance(parser); // Consume '('

    if (parser->current.type != TOKEN_STRING) {
        printf("Error: Expected a string message inside alert().\n");
        exit(1);
    }
    ASTNode *action_node = make_action(parser->current.text);
    advance(parser); // Consume string

    if (parser->current.type != TOKEN_RPAREN) {
        printf("Error: Expected ')' after alert message.\n");
        exit(1);
    }
    advance(parser); // Consume ')'

    return action_node;
}

// Parses a statement, e.g., if tcp.syn then alert("...")
ASTNode *parse_statement(Parser *parser) {
    if (parser->current.type != TOKEN_IF) {
        printf("Error: Expected 'if' keyword to start a rule.\n");
        exit(1);
    }
    advance(parser); // Consume 'if'

    if (parser->current.type != TOKEN_IDENT) {
        printf("Error: Expected a condition identifier (e.g., 'tcp.syn') after 'if'.\n");
        exit(1);
    }
    ASTNode *condition_node = make_condition(parser->current.text);
    advance(parser); // Consume identifier

    if (parser->current.type != TOKEN_THEN) {
        printf("Error: Expected 'then' keyword after condition.\n");
        exit(1);
    }
    advance(parser); // Consume 'then'

    ASTNode *action_node = parse_action(parser);

    return make_if_statement(condition_node, action_node);
}

// Parses the entire program as a list of statements
ASTNode *parse_program(Parser *parser) {
    ASTNode *program_node = make_node(NODE_PROGRAM);
    program_node->program.statements = NULL;

    ASTNode *head = NULL;
    ASTNode *current_stmt = NULL;

    while (parser->current.type != TOKEN_EOF) {
        ASTNode *stmt = parse_statement(parser);
        if (!head) {
            head = stmt;
            current_stmt = stmt;
        } else {
            current_stmt->next = stmt;
            current_stmt = stmt;
        }
    }

    program_node->program.statements = head;
    return program_node;
}


// --- AST Utility Functions (Updated) ---

void print_ast(ASTNode *node, int indent) {
    if (!node) return;

    for (int i = 0; i < indent; i++) printf("  ");

    switch (node->type) {
        case NODE_PROGRAM:
            printf("Program\n");
            print_ast(node->program.statements, indent + 1);
            break;
        case NODE_IF_STATEMENT:
            printf("IfStatement\n");
            print_ast(node->if_stmt.condition, indent + 1);
            print_ast(node->if_stmt.action, indent + 1);
            break;
        case NODE_CONDITION:
            printf("Condition(%s)\n", node->condition.identifier);
            break;
        case NODE_ACTION:
            printf("Action(alert: \"%s\")\n", node->action.message);
            break;
    }

    // Print next statement at the same level
    if (node->next) {
        print_ast(node->next, indent);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;

    // Free children first
    switch (node->type) {
        case NODE_PROGRAM:
            free_ast(node->program.statements);
            break;
        case NODE_IF_STATEMENT:
            free_ast(node->if_stmt.condition);
            free_ast(node->if_stmt.action);
            break;
        case NODE_CONDITION:
        case NODE_ACTION:
            // No children to free for these node types
            break;
    }

    // Free the next statement in the list
    free_ast(node->next);

    // Free the node itself
    free(node);
}