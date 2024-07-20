#ifndef RECIEVER_HPP
#define RECIEVER_HPP

#include <string>
#include <vector>
#include <arpa/inet.h>
#include "Client.hpp"

class Reciever : public Client{
public:
    Reciever(const std::string& serverIp, const std::string& clientIP, int port);
    void start();
    void handleRecievingTask();
    void sendAcknowledgeMessage();
    void handleIncomingMessage(std::vector<std::string> &rec);

private:
    std::vector<std::string> recievedSequences;
    int lastRecievedSequence;
};

#endif // RECIEVER_HPP