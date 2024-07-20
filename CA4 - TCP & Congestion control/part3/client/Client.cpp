#include "Reciever.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"
#include "Client.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Client::Client(const string& _serverIp, const string& _clientIP, int _port)
    : serverIp(_serverIp), clientIP(_clientIP), port(_port) {}


void Client::sendMessage(string msg)
{
    sendto(sockfd, msg.c_str(), msg.length(), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
}

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

string Client::recieveMessage(int timeoutSeconds)
{
    char buffer[BUFFER_SIZE];
    socklen_t len;
    fd_set readfds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);
    timeout.tv_sec = timeoutSeconds;
    timeout.tv_usec = 0;
    int retval = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
    if (retval == -1)
    {
        // Handle error
    } 
    else if (retval) 
    {
        int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        return string(buffer);
    } 
    else
        return "TIMEOUT";
}

void Client::setupSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
}

void Client::sendGreeting() {
    string greeting = "GREETING " + clientIP;
    sendMessage(greeting);
    string answer = recieveMessage(100);
    cout << "Server response: " << answer << endl;
}
