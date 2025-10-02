#include <stdio.h>
#include "lexer.h"
#include "parser.h"

int main() {
    const char *code = "if tcp.syn then alert(\"Possible scan\")";

    Lexer lexer;
    init_lexer(&lexer, code);

    AST ast;
    if (parse_rule(&lexer, &ast) == 0) {
        printf("Parsed successfully!\n");
        printf("Condition: %s\n", ast.condition);
        printf("Action   : %s\n", ast.action);
        printf("Argument : %s\n", ast.argument);
    } else {
        printf("Parsing failed.\n");
    }

    return 0;
}

