#include "Reciever.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"
#include "Reciever.hpp"

using namespace std;

#define BUFFER_SIZE 1024

Reciever::Reciever(const string& _serverIp, const string& _clientIP, int _port)
: Client(_serverIp , _clientIP  ,_port) 
{
    lastRecievedSequence = 0;
}

void Reciever::start() {
    setupSocket();
    sendGreeting();
    handleRecievingTask();
}

void Reciever::handleRecievingTask() 
{
    while (true) 
    {
        recievedSequences.clear();
        for(int i = 0 ; i < 16 ; i++)
        {
            string recievedMessage = recieveMessage(3);
            if(recievedMessage!="TIMEOUT")
            {
                vector<string> rec = splitString(recievedMessage, ' ');
                handleIncomingMessage(rec);
            }
            else
                break;
        }
        sendAcknowledgeMessage();
    }
}

void Reciever::sendAcknowledgeMessage()
{
    if(recievedSequences.size() > 0)
    {
        string answer = "MESSAGE 192.168.1.3 192.168.1.1 ";
        for (int i = 0; i < recievedSequences.size(); i++)
            answer += "-" + recievedSequences[i];
        sendMessage(answer);
    }
}

void Reciever::handleIncomingMessage(std::vector<std::string> &rec)
{
    if (rec[0] == "MESSAGE")
    {
        vector<string> body = splitString(rec[2], '-');
        recievedSequences.push_back(body[0]);
    }
}
