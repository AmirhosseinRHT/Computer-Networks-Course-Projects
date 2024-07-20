#ifndef _CLIENT_HPP
#define _CLIENT_HPP

#include <iostream>
#include <string>
#include<cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include"defs.hpp"


class Client {
public:
    Client(const std::string& serverIP, int serverPort);
    void sendMessage(const std::string& message ,int seq);
    void handshake();
    std::string recieveMessage(int timeoutSeconds);

private:
    int sockfd;
    sockaddr_in server_addr;
};



#endif