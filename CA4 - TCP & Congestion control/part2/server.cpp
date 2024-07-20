#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include<stdlib.h>
#include<cstring>
#include <thread>
#include<chrono>
#include <mutex>
#include <queue>
#include <condition_variable>
#include "defs.hpp"
#define BUFFER_SIZE 1024

class Server {
public:
    Server(int port) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
            std::cerr << "Failed to bind socket" << std::endl;
            return;
        }

        std::cout << "Server started on port " << port << std::endl;
    }

    void run() {
        char *buffer;
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        while (true) {
            Packet packet;
            int recv_len = recvfrom(sockfd,(void *) &packet, sizeof(Packet), 0, (struct sockaddr*)&client_addr, &client_len);
            if (recv_len < 0) {
                std::cerr << "Failed to receive data" << std::endl;
                continue;
            }
            if( (packet.syn & (!packet.ack))){
                printl("SYN recived");
                handle_handshake(&packet ,&client_addr ,client_len);
                   
            }
        }
    }

    void handle_handshake(Packet *packet ,sockaddr_in *client_addr ,socklen_t client_len){
        int ack_seq = packet->syn_seq +1;
        Packet *syn_ack_packet =make_syn_ack_packet(10 , ack_seq);
        Packet ack_packet;
        sendto(sockfd,(void *)syn_ack_packet, sizeof(Packet), 0, (struct sockaddr*)client_addr, client_len);
        int recv_len = recvfrom(sockfd,(void *) &ack_packet, sizeof(Packet), 0, (struct sockaddr*)client_addr, &client_len);
        if(ack_packet.ack & (!ack_packet.syn)){
            printl("ACK recived");
        }

        // new thread
        std::thread client(&Server::handle_client ,this ,std::ref(*client_addr), client_len);
        client.join();
        
    }

    void handle_client(struct sockaddr_in client_addr,socklen_t client_len){
        
        auto client_port = client_addr.sin_port;
        int num_complete = 0;
        int *recieve_list= new int[500];
        std::memset(recieve_list , 0,500 * sizeof(int));
        while (true) {
            Packet packet;
            int recv_len = recvfrom(sockfd, (void*)&packet, sizeof(Packet), 0, (struct sockaddr*)&client_addr, &client_len);
            if (recv_len < 0) {
                // Handle error
                continue;
            }
            
            if(packet.data_size >= BUFFER_SIZE)
                continue;
            std::cout << "Received message: " << packet.data << std::endl;
            std::string ack;
            ack = std::to_string(packet.syn_seq);

            Packet ans;
            ans.ack_seq = packet.syn_seq;
            if(packet.syn_seq == num_complete + 1){
                num_complete++;
                recieve_list[num_complete] =1;
                ack = std::to_string(packet.syn_seq);
            }
            else{ //handle packets droped
                recieve_list[packet.syn_seq] =1;
                for(int s= num_complete+1 ; s < packet.syn_seq; s++){
                    if(!recieve_list[s]){
                        ans.ack_seq = s;
                        ack ="N"+ std::to_string(s);    
                    }
                }
            }
            ans.ack =1;
            ans.psh = 1;
            ans.syn =0;
            ans.data_size =ack.size();
            std::memcpy(ans.data ,ack.c_str() ,ack.size());
            sendto(sockfd, &ans, sizeof(Packet), 0, (struct sockaddr*)&client_addr, client_len);
            std::cout << "Sent ACK to client " << packet.syn_seq << std::endl;
            std::this_thread::sleep_for(std::chrono::microseconds(1000));
        }
    }

    

private:
    int sockfd;
    sockaddr_in server_addr;
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    Server server(port);
    server.run();

    return 0;
}