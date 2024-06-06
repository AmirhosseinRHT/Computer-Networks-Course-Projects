#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
#include "tableRow.h"
#include "packet.hpp"

class Router : public Node
{
public:
    explicit Router(IP _ip ,IPversion v , int _portQueueSize , bool isEdge=false);
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
    void updatePacketLogs(QSharedPointer<Packet> p , QString log);
    bool isEdgeRouter;
    void printRoutingTable();
    void sendGreetingPacket();
    void handleGreetingPacket(QSharedPointer<Packet> p , int portNum);
    void handleDhcpRequest(QSharedPointer<Packet> p, int portNum);
    void forwardPacket(QSharedPointer<Packet> p);
private:
    QMap<IP ,  route> routingTable;
    QVector<forward *> forwardingTable;
    QVector<QPair<int,IP>> assignedIPs;
    void updateDistanceVec(QSharedPointer<Packet> p);
    void sendRouteTebleInfo();
    QString dataOfRoutingTable(NodeType nextHopType);
    bool routingTableChanged;

public slots:
    void onClock(NetworkState ns) override;
    void printTable(QString _ip);
};

#endif // ROUTER_HPP
