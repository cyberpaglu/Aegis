// interpreter.c
#include <stdio.h>
#include <string.h>
#include "interpreter.h"

void evaluate_rule(AST *rule, Packet *pkt) {
    int condition_met = 0;

    // Check condition field
    if (strcmp(rule->condition, "tcp.syn") == 0) {
        if (pkt->tcp_syn) condition_met = 1;
    } else if (strcmp(rule->condition, "udp.port") == 0) {
        if (pkt->udp_port > 0) condition_met = 1;
    } else if (strcmp(rule->condition, "dns.query") == 0) {
        if (strstr(pkt->dns_query, rule->argument)) condition_met = 1;
    } else if (strcmp(rule->condition, "http.url") == 0) {
        if (strstr(pkt->http_url, rule->argument)) condition_met = 1;
    }

    // Trigger action
    if (condition_met) {
        if (strcmp(rule->action, "alert") == 0) {
            printf("[ALERT] %s | %s -> %s\n",
                   rule->argument,
                   pkt->src_ip,
                   pkt->dst_ip);
        }
    }
}

