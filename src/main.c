#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For getopt
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
#include "packet.h"

void print_usage(const char *prog_name) {
    printf("Usage: %s -r <rules_file> [-i <interface> | -f <pcap_file>]\n", prog_name);
    printf("Options:\n");
    printf("  -r <rules_file>   Path to the file containing detection rules.\n");
    printf("  -i <interface>    Live network interface to capture packets from.\n");
    printf("  -f <pcap_file>    .pcap file to read packets from.\n");
    printf("  -h                Display this help message.\n");
}

char* read_file_content(const char* path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("Error opening rules file");
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);

    char *code = malloc(size + 1);
    if (!code) {
        fprintf(stderr, "Error: Could not allocate memory for file content.\n");
        fclose(fp);
        return NULL;
    }

    fread(code, 1, size, fp);
    code[size] = '\0';
    fclose(fp);
    return code;
}

int main(int argc, char *argv[]) {
    char *rules_file = NULL;
    char *interface = NULL;
    char *pcap_file = NULL;
    int opt;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    while ((opt = getopt(argc, argv, "r:i:f:h")) != -1) {
        switch (opt) {
            case 'r':
                rules_file = optarg;
                break;
            case 'i':
                interface = optarg;
                break;
            case 'f':
                pcap_file = optarg;
                break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }

    if (!rules_file) {
        fprintf(stderr, "Error: Rules file is mandatory. Use -r <rules_file>.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (interface && pcap_file) {
        fprintf(stderr, "Error: Please specify either a live interface (-i) or a pcap file (-f), not both.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!interface && !pcap_file) {
        fprintf(stderr, "Error: You must specify a source, either a live interface (-i) or a pcap file (-f).\n");
        print_usage(argv[0]);
        return 1;
    }

    printf("Configuration:\n");
    printf("  Rules File: %s\n", rules_file);
    if (interface) {
        printf("  Interface: %s\n", interface);
    }
    if (pcap_file) {
        printf("  PCAP File: %s\n", pcap_file);
    }
    printf("\n");

    // --- Read and process the rules file ---
    printf("Loading rules from %s...\n", rules_file);
    char* rules_content = read_file_content(rules_file);
    if (!rules_content) {
        return 1;
    }

    // Initialize lexer & parser
    Lexer lexer;
    init_lexer(&lexer, rules_content);

    Parser parser;
    init_parser(&parser, &lexer);

    // Parse program -> AST
    ASTNode *root = parse_program(&parser);
    printf("Rules parsed successfully. AST created.\n\n");

    // Print the AST for debugging
    printf("--- Abstract Syntax Tree ---\n");
    print_ast(root, 0);
    printf("--------------------------\n\n");

    // --- Start packet processing ---
    if (interface) {
        printf("Starting live capture on interface %s...\n", interface);
        start_capture(interface, root);
    } else if (pcap_file) {
        printf("Processing pcap file %s...\n", pcap_file);
        process_pcap_file(pcap_file, root);
    }

    // Cleanup
    free_ast(root);
    free(rules_content);

    return 0;
}
