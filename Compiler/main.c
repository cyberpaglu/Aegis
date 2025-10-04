#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <source file>\n", argv[0]);
        return 1;
    }

    // Read source code from file
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *code = malloc(size + 1);
    fread(code, 1, size, fp);
    code[size] = '\0';
    fclose(fp);

    // Initialize lexer & parser
    Lexer lexer;
    init_lexer(&lexer, code);

    Parser parser;
    init_parser(&parser, &lexer);

    // Parse program → AST
    ASTNode *root = parse_program(&parser);

    // Print AST (debugging)
    print_ast(root, 0);
    printf("Result: %d\n", eval(root));
    // Cleanup
    free_ast(root);
    free(code);

    return 0;
}
