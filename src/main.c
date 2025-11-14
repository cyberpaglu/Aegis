#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // For getopt
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"
// It's likely we'll need a function from packet.h here in the future
// #include "packet.h"

void print_usage(const char *prog_name) {
    printf("Usage: %s -r <rules_file> [-i <interface> | -f <pcap_file>]\n", prog_name);
    printf("Options:\n");
    printf("  -r <rules_file>   Path to the file containing detection rules.\n");
    printf("  -i <interface>    Live network interface to capture packets from.\n");
    printf("  -f <pcap_file>    .pcap file to read packets from.\n");
    printf("  -h                Display this help message.\n");
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

    printf("Configuration:\n");
    printf("  Rules File: %s\n", rules_file);
    if (interface) {
        printf("  Interface: %s\n", interface);
    }
    if (pcap_file) {
        printf("  PCAP File: %s\n", pcap_file);
    }

    // --- Placeholder for the rest of the IDS logic ---
    // The next step would be to read the rules file, parse it,
    // and then start capturing and processing packets.
    // For now, this confirms the argument parsing works.

    printf("\nArgument parsing successful. The full IDS logic can now be integrated.\n");

    return 0;
}