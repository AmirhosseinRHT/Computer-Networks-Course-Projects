#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
#include "tableRow.h"
#include "packet.hpp"

class Router : public Node
{
public:
    explicit Router(IP _ip ,IPversion v);
    ~Router();
    forward* createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID, int queueSize);
    void sendToPort(int portNum , QSharedPointer<Packet> pack);
    Port* getPort(int num);
    Port* getPort(IP ip);
    IP requestIP(int portNum);
    bool giveBackIP(QString ip);
private:
    QVector<route> routingTable;
    QVector<forward *> forwardingTable;
    QVector<QPair<int,IP>> assignedIPs;
};

#endif // ROUTER_HPP
