#include <stdio.h>
#include <string.h>
#include "interpreter.h"

// Evaluates the AST against a parsed packet.
void eval(ASTNode *node, Packet* p) {
    if (!node || !p) return;

    switch (node->type) {
        case NODE_PROGRAM:
            // Evaluate all statements in the program
            eval(node->program.statements, p);
            break;

        case NODE_IF_STATEMENT: {
            // The core logic for a single rule.
            int condition_met = 0;
            const char* condition_ident = node->if_stmt.condition->condition.identifier;

            // --- Condition Matching Logic ---
            // This is where we check the packet's properties against the rule's condition.
            if (strcmp(condition_ident, "tcp.syn") == 0 && p->tcp_syn) {
                condition_met = 1;
            } else if (strcmp(condition_ident, "tcp.ack") == 0 && p->tcp_ack) {
                condition_met = 1;
            }
            // Add more conditions here (e.g., for DNS, HTTP, etc.)
            // else if (strcmp(condition_ident, "dns.query") == 0 && strlen(p->dns_query) > 0) {
            //     condition_met = 1;
            // }


            if (condition_met) {
                // If the condition is met, execute the action.
                const char* message = node->if_stmt.action->action.message;
                // For now, the only action is to print an alert.
                printf("[ALERT] Rule triggered for packet %s -> %s. Message: %s\n",
                       p->src_ip, p->dst_ip, message);
            }
            break;
        }

        case NODE_CONDITION:
        case NODE_ACTION:
            // These nodes are handled within the NODE_IF_STATEMENT case.
            // No action is needed when encountering them directly.
            break;
    }

    // A program is a linked list of statements.
    // Move to the next statement and evaluate it.
    if (node->next) {
        eval(node->next, p);
    }
}