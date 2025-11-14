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
sudo ./ids -i eth0 -r rules.dsl
```

To capture from a `.pcap` file:
```bash
./ids -f test.pcap -r rules.dsl
```

# Development Conventions

*   **Language:** C
*   **Parsing:** Custom Lexer and Recursive Descent Parser (no external tools like Flex/Bison).
*   **Rule Definition:** Custom Domain-Specific Language (DSL) defined in plain text files (e.g., `rules.dsl`).
*   **Testing:** Testing can be done with `.pcap` files or by generating traffic with tools like `nmap`, `curl`, or `dig`.

# Key Files and Directories

*   `Compiler/`: Contains the core source code for the IDS, including the lexer, parser, interpreter, packet capture, and main program entry point.
    *   `lexer.h / lexer.c`: Tokenizer for the DSL.
    *   `parser.h / parser.c`: Parser and AST builder.
    *   `interpreter.h / .c`: Rule execution engine.
    *   `packet.h / packet.c`: Packet capture and parsing.
    *   `main.c`: Program entry point.
    *   `rules.dsl`: Example detection rules written in the custom DSL.
    *   `test.mylang`: Another example file for the custom language.
*   `README.md`: Provides a detailed overview, features, project structure, example DSL, getting started guide, example output, development roadmap, testing information, and license.
*   `GEMINI.md`: This file, summarizing the project for AI interaction.