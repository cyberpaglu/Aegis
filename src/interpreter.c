#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

int eval(ASTNode *node) {
    if (!node) return 0;

    switch (node->type) {
        case NODE_NUMBER:
            return node->value;

        case NODE_BINARY: {
            int left = eval(node->binary.left);
            int right = eval(node->binary.right);
            switch (node->binary.op) {
                case TOKEN_PLUS:  return left + right;
                case TOKEN_MINUS: return left - right;
                case TOKEN_STAR:  return left * right;
                case TOKEN_SLASH: return right != 0 ? left / right : 0;
                default:
                    printf("Unknown operator\n");
                    exit(1);
            }
        }

        default:
            printf("Unknown AST node\n");
            exit(1);
    }
}
