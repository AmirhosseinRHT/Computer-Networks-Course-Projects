#ifndef SENDER_HPP
#define SENDER_HPP

#include <string>
#include <arpa/inet.h>
#include "Client.hpp"
#include "../GBN/senderGBN.hpp"

class Sender : Client {
public:
    Sender(const std::string& serverIp, const std::string& clientIP, int port);
    ~Sender();
    void start();
    void startSendingTask();

private:
    SenderGoBackN * gbn;
};

#endif // SENDER_HPP