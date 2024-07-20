#include "Client.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sstream>
#include "../defines/defs.hpp"
#include "Sender.hpp"


using namespace std;

#define BUFFER_SIZE 1024

Sender::Sender(const string& _serverIp, const string& _clientIP, int _port)
: Client(_serverIp , _clientIP  ,_port) 
{
    gbn = new SenderGoBackN(16 , 100);
}

Sender::~Sender()
{
    delete gbn;
}

void Sender::start() 
{
    setupSocket();
    sendGreeting();
    startSendingTask();
}

void Sender::startSendingTask() 
{
    while(gbn->getCurrentIndex() <= gbn->getTotalFrames())
    {
        for(int k=gbn->getCurrentIndex(); k < gbn->getCurrentIndex() + gbn->getSeqNum() && k <= gbn->getTotalFrames() ; k++)
        {
            cout << "Sending Frame " << k << " ..." << endl;
            sendMessage("MESSAGE 192.168.1.1 192.168.1.3 -"+ to_string(k) + "-hello") ;
            gbn->increaseTotalTransaction();
        }
        string recievedAcks = recieveMessage(5);
        cout << "ACKS: " << recievedAcks << endl;

        gbn->updateIndex(splitString(recievedAcks , '-'));
        sleep(1);
    }
    cout << "program finnished. number of Transactions: " << gbn->getTotalTransaction() << endl;
    return;
}