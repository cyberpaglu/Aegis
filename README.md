🛡️ Custom Intrusion Detection System (IDS) with Scripting Language in C

📌 Overview

This project is a lightweight Intrusion Detection System (IDS) written
in C, capable of:

-   Capturing packets from the network,
-   Parsing user-defined detection rules from a custom Domain-Specific
    Language (DSL),
-   Detecting suspicious activity,
-   Triggering alerts/logging.

The design is inspired by Snort, Suricata, and Zeek, but built from
scratch for educational purposes.
Instead of using YAML rules or a full scripting engine, this IDS uses a
mini scripting language defined in plain text.

------------------------------------------------------------------------

✨ Features

-   Packet capture with libpcap (or raw sockets).
-   Custom DSL for writing rules (e.g.,
    if tcp.syn then alert("Scan detected")).
-   Lexer + Recursive Descent Parser (no Flex/Bison).
-   Abstract Syntax Tree (AST) for rule evaluation.
-   Interpreter that executes actions (alert, log, drop) on matching
    packets.
-   Support for multiple rules loaded from a file.
-   Extensible grammar (operators, logical conditions, protocol fields).

------------------------------------------------------------------------

🛠️ Project Structure

    .
    ├── src/
    │   ├── lexer.h / lexer.c           # Tokenizer for the DSL
    │   ├── parser.h / parser.c         # Parser and AST builder
    │   ├── interpreter.h / interpreter.c # Rule execution engine
    │   ├── packet.h                    # Packet capture/parsing (to be implemented)
    │   └── main.c                      # Main program entry point
    ├── rules/
    │   └── rules.dsl                   # Example detection rules
    ├── bin/
    │   └── ids                         # The compiled executable
    ├── obj/                            # Intermediate object files
    ├── Makefile                        # The build script
    └── README.md                       # This file

------------------------------------------------------------------------

📜 Example DSL

Example rules.dsl file:

    rule suspicious_scan:
        if tcp.syn then alert("TCP SYN scan detected")

    rule bad_dns:
        if dns.query contains "evil.com" then alert("Malicious DNS query")

-   Keywords: rule, if, then
-   Events: tcp.syn, dns.query
-   Operators: ==, contains
-   Actions: alert, log, drop

------------------------------------------------------------------------

🚀 Getting Started

1. Clone Repo

    git clone https://github.com/yourusername/ids-project.git
    cd ids-project

2. Install Dependencies

Make sure you have: - GCC/Clang (C compiler) - libpcap development
libraries

On Ubuntu/Debian:

    sudo apt update
    sudo apt install build-essential libpcap-dev

On Fedora:

    sudo dnf install gcc libpcap-devel

3. Build

    make

This should produce an executable (e.g., ./ids).

4. Run

Capture from a live interface:

    sudo ./ids -i eth0 -r rules.dsl

Capture from a .pcap file:

    ./ids -f test.pcap -r rules.dsl

------------------------------------------------------------------------

📊 Example Output

    [ALERT] TCP SYN scan detected from 192.168.1.50 to 192.168.1.10
    [ALERT] Malicious DNS query: evil.com from 10.0.0.5

------------------------------------------------------------------------

🛣️ Development Roadmap

-   ✅ Phase 1: Packet capture
-   ✅ Phase 2: Lexer & Parser
-   ✅ Phase 3: Interpreter with simple actions
-   🔄 Phase 4: Multiple rules from file
-   🔄 Phase 5: Extended grammar (logical ops, comparisons)
-   🔮 Future: Bytecode VM, plugins, web dashboard

------------------------------------------------------------------------

🧪 Testing

You can test with .pcap files:

    ./ids -f sample.pcap -r rules.dsl

Or generate traffic with tools like nmap, curl, or dig.

------------------------------------------------------------------------

⚡ Stretch Goals

-   Compile DSL to bytecode for faster execution.
-   Multi-threading for high-speed packet rates.
-   Plugin API (Lua/Python).
-   Web dashboard for visualization.

------------------------------------------------------------------------

📛 Suggested Project Names

-   Sentinel 🛡️
-   AegisLang ⚔️
-   PacketScribe ✍️
-   RuleWire 🔌
-   Vigil 👁️

------------------------------------------------------------------------

📜 License

MIT License.
Free to use, modify, and share.

------------------------------------------------------------------------

👤 Author

Developed by [Your Name]
Inspired by real IDS tools but built for learning and research.
