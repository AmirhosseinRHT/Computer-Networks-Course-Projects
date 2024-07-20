#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Server::Server(int port) : port(port) {}

void Server::start() {
    setupSocket();
    bindSocket();
    cout << "Server listening on port " << port << endl;
    receiveAndRespond();
}

void Server::setupSocket() {
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
}

void Server::bindSocket() {
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in Server::recieveMessage(string &msg)
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_in cliaddr;
    socklen_t len = sizeof(cliaddr);
    int n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    msg = string(buffer);
    return cliaddr;
}

void Server::receiveAndRespond() 
{
    string message;
    struct sockaddr_in cliaddr;
    while (true) 
    {
        cliaddr = recieveMessage(message);
        vector<string >splitted = splitString(message , ' ');
        if (splitted[0] == "GREETING") 
            handleGreeting(splitted[1], cliaddr);
        else if (splitted[0] == "MESSAGE") 
            handleMessage(splitted[1], splitted[2], splitted[3]);
    }
}

void Server::handleGreeting(const string& clientIP, const struct sockaddr_in& clientAddr) 
{
    clients[clientIP] = clientAddr;
    cout << "New client connected: " << clientIP << endl;
    sendMessageTo(clientIP , "GREETING_ACK");
}

void Server::sendMessageTo(std::string destination , std::string msg)
{
    sendto(sockfd, msg.c_str(), msg.length(), MSG_CONFIRM, 
        (const struct sockaddr *)&clients[destination], sizeof(clients[destination]));
}

void Server::handleMessage(const string& sourceIP, const string& destIP, const string& message) 
{
    cout << "Message from " << sourceIP << " to " << destIP << ": " << message << endl;

    if (clients.find(destIP) != clients.end())
        sendMessageTo(destIP , "MESSAGE " + sourceIP + " " + message);
    else 
        cout << "Destination client not found: " << destIP << endl;
    
}