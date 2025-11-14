#include "packet.h"
#include "interpreter.h"
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/if_ether.h>
#include <arpa/inet.h>

// Callback function to process each captured packet
void got_packet(u_char *args, const struct pcap_pkthdr *header, const u_char *packet_data) {
    ASTNode *ast_root = (ASTNode *)args;
    Packet p;
    memset(&p, 0, sizeof(Packet)); // Zero out the packet struct

    // --- Packet Parsing ---
    const struct ether_header *eth_header;
    const struct ip *ip_header;
    const struct tcphdr *tcp_header;
    int size_eth = sizeof(struct ether_header);
    int size_ip;
    int size_tcp;

    eth_header = (struct ether_header *)(packet_data);

    // We are only interested in IP packets
    if (ntohs(eth_header->ether_type) != ETHERTYPE_IP) {
        return;
    }

    ip_header = (struct ip*)(packet_data + size_eth);
    size_ip = ip_header->ip_hl * 4;
    if (size_ip < 20) {
        // Invalid IP header length
        return;
    }

    // Copy IP addresses
    inet_ntop(AF_INET, &(ip_header->ip_src), p.src_ip, sizeof(p.src_ip));
    inet_ntop(AF_INET, &(ip_header->ip_dst), p.dst_ip, sizeof(p.dst_ip));

    // We are only interested in TCP packets for now
    if (ip_header->ip_p != IPPROTO_TCP) {
        // In the future, handle UDP, ICMP, etc.
        return;
    }

    tcp_header = (struct tcphdr*)(packet_data + size_eth + size_ip);
    size_tcp = tcp_header->th_off * 4;
    if (size_tcp < 20) {
        // Invalid TCP header length
        return;
    }

    // Set TCP flags in our custom Packet struct
    p.tcp_syn = tcp_header->th_flags & TH_SYN ? 1 : 0;
    p.tcp_ack = tcp_header->th_flags & TH_ACK ? 1 : 0;

    // --- Interpreter ---
    // Now that the packet is parsed and the struct is filled,
    // call the interpreter to evaluate the rules.
    eval(ast_root, &p);
}

// Function to start capturing packets from a live interface
void start_capture(const char *interface, ASTNode *root) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    printf("Attempting to open device %s...\n", interface);
    handle = pcap_open_live(interface, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open device %s: %s\n", interface, errbuf);
        exit(EXIT_FAILURE);
    }
    printf("Device %s opened successfully.\n", interface);

    printf("Starting packet capture loop...\n");
    if (pcap_loop(handle, -1, got_packet, (u_char *)root) < 0) {
        fprintf(stderr, "pcap_loop() failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        exit(EXIT_FAILURE);
    }

    printf("Capture finished.\n");
    pcap_close(handle);
}

// Function to process packets from a .pcap file
void process_pcap_file(const char *filename, ASTNode *root) {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    printf("Attempting to open pcap file %s...\n", filename);
    handle = pcap_open_offline(filename, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "Couldn't open pcap file %s: %s\n", filename, errbuf);
        exit(EXIT_FAILURE);
    }
    printf("File %s opened successfully.\n", filename);

    printf("Starting packet processing from file...\n");
    if (pcap_loop(handle, -1, got_packet, (u_char *)root) < 0) {
        fprintf(stderr, "pcap_loop() failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        exit(EXIT_FAILURE);
    }

    printf("Finished processing pcap file.\n");
    pcap_close(handle);
}
