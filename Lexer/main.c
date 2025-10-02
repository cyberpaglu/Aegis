// main.c
#include <stdio.h>
#include "lexer.h"

int main() {
    const char *code = "if tcp.syn then alert(\"Possible scan\")";
    Lexer lexer;
    init_lexer(&lexer, code);

    Token tok;
    do {
        tok = next_token(&lexer);
        printf("Token: %-12d Text: %s\n", tok.type, tok.text);
    } while (tok.type != TOKEN_EOF);

    return 0;
}

