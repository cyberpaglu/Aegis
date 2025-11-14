# Project Overview

This project is a Custom Intrusion Detection System (IDS) implemented in C. It features a custom scripting language (DSL) for defining detection rules, a lexer, a recursive descent parser, an Abstract Syntax Tree (AST) builder, and an interpreter. The IDS is capable of capturing network packets, evaluating user-defined rules, and triggering actions like alerts or logging for suspicious activities. It's inspired by tools like Snort, Suricata, and Zeek, but built from scratch for educational purposes.

**Main Technologies:** C, libpcap (for packet capture).
**Architecture:** The system is structured around a lexer, parser, AST, and interpreter for its custom DSL, integrated with a packet capture and parsing module.

# Building and Running

The project uses a `Makefile` for building.

## Dependencies

*   GCC/Clang (C compiler)
*   libpcap development libraries

**On Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential libpcap-dev
```

**On Fedora:**
```bash
sudo dnf install gcc libpcap-devel
```

## Build Commands

To compile the project:
```bash
make
```
This will produce an executable (e.g., `./ids` or `./mini_ids`).

## Run Commands

To capture from a live network interface:
```bash
sudo ./bin/ids -i eth0 -r rules/rules.dsl
```

To capture from a `.pcap` file:
```bash
./bin/ids -f test.pcap -r rules/rules.dsl
```

# Development Conventions

*   **Language:** C
*   **Parsing:** Custom Lexer and Recursive Descent Parser (no external tools like Flex/Bison).
*   **Rule Definition:** Custom Domain-Specific Language (DSL) defined in plain text files (e.g., `rules.dsl`).
*   **Testing:** The program can be tested by running it with a rules file and a `.pcap` file.

# Current Status

The project has undergone a major refactoring:
1.  **File Structure:** The project has been organized into `src`, `bin`, `obj`, and `rules` directories.
2.  **Build System:** A `Makefile` has been added to automate the build process.
3.  **Core Engine:** The lexer, parser, and interpreter have been rewritten to correctly process the IDS rule language (`if...then...alert`). The command-line interface in `main.c` is functional.
4.  **Packet Processing:** Real packet capture and basic parsing (Ethernet, IP, TCP) have been implemented in `packet.c`. The interpreter now evaluates rules against actual packet data.

# Key Files and Directories

*   `src/`: Contains the core C source code.
    *   `lexer.h / .c`: Tokenizer for the DSL.
    *   `parser.h / .c`: Parser and AST builder.
    *   `interpreter.h / .c`: Rule execution engine.
    *   `packet.h / .c`: Packet capture and parsing logic.
    *   `main.c`: Main program entry point with CLI argument parsing.
*   `rules/`: Contains the DSL rule files.
*   `bin/`: Contains the compiled `ids` executable.
*   `obj/`: Contains intermediate object files.
*   `Makefile`: The build script.
*   `README.md`: The original, detailed project documentation.
*   `GEMINI.md`: This file, summarizing the project for AI interaction.