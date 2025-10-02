#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "packet.h"

#define MAX_RULES 50

int main() {
    AST rules[MAX_RULES];
    int rule_count = 0;

    // Open rule file
    FILE *fp = fopen("rules.dsl", "r");
    if (!fp) {
        perror("Failed to open rules.dsl");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        // Skip empty lines
        if (strlen(line) <= 1) continue;

        Lexer lexer;
        init_lexer(&lexer, line);

        AST rule;
        if (parse_rule(&lexer, &rule) == 0) {
            rules[rule_count++] = rule;
            if (rule_count >= MAX_RULES) break;
        }
    }
    fclose(fp);

    // Example packets
    Packet packets[3] = {
        {"192.168.1.100", "192.168.1.10", 1, 0, 0, "", ""},
        {"10.0.0.5", "10.0.0.1", 0, 0, 0, "evil.com", ""},
        {"172.16.0.2", "172.16.0.1", 0, 0, 0, "", "http://example.com/malware/page"}
    };

    int num_packets = 3;

    // Evaluate each rule against each packet
    for (int i = 0; i < num_packets; i++) {
        for (int j = 0; j < rule_count; j++) {
            evaluate_rule(&rules[j], &packets[i]);
        }
    }

    return 0;
}

