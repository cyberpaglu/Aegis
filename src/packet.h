// packet.h
#ifndef PACKET_H
#define PACKET_H

#include "parser.h" // For ASTNode

typedef struct {
    char src_ip[16];
    char dst_ip[16];
    int tcp_syn;   // 1 if SYN flag is set
    int tcp_ack;
    int udp_port;
    char dns_query[128];
    char http_url[128];
} Packet;

void start_capture(const char *interface, ASTNode *root);
void process_pcap_file(const char *filename, ASTNode *root);

#endif

