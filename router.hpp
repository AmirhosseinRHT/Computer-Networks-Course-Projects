#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
#include "tableRow.h"
#include "packet.hpp"

class Router : public Node
{
public:
    explicit Router(IP _ip ,IPversion v , int _portQueueSize);
    ~Router();
    forward* createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID, int queueSize);
    void sendToPort(int portNum , QSharedPointer<Packet> pack);
    Port* getPort(int num);
    Port* getPort(IP ip);
    IP requestIP(int portNum);
    bool giveBackIP(QString ip);
    IP convertIPv4ToIPv6(IP ipv4Address);
    IP convertIPv6ToIPv4(IP ipv6Address);
    void roundRobinPacketHandler();
    void handleDequeuedPacket(QSharedPointer<Packet> p , int portNum);
private:
    QMap<IP ,  route> routingTable;
    QVector<forward *> forwardingTable;
    QVector<QPair<int,IP>> assignedIPs;
    void updateDistanceVec(QSharedPointer<Packet> p , int portNum);
public slots:
    void onClock() override;
};

#endif // ROUTER_HPP
