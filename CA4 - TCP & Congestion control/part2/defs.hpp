#ifndef DEFS_HPP
#define DEFS_HPP


#include<string>
#include<iostream>

#define MESSAGE_SIZE 1024
#define BUFFER_SIZE 1024


struct Packet{
    unsigned int syn_seq;
    unsigned int ack_seq;
    unsigned short syn;
    unsigned short ack;
    unsigned short psh;
    unsigned int data_size;
    char data[1024];      
};


void printl(std::string s);
Packet *make_syn_ack_packet(int seq , int ack_seq);
Packet *make_syn_packet(int seq , int ack_seq);
Packet *make_ack_packet(int seq , int ack_seq);


#endif
