#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <arpa/inet.h>

class Client {
public:
    Client(const std::string& serverIp, const std::string& clientIP, int port);
    void setupSocket();
    void sendGreeting();
    void sendMessage(std::string msg);
    std::string recieveMessage(int timeoutSeconds);

protected:
    int sockfd;
    struct sockaddr_in servaddr;
    std::string serverIp;
    std::string clientIP;
    int port;
};

#endif // CLIENT_HPP