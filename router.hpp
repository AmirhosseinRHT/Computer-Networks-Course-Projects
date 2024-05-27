#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
#include "tableRow.h"
#include "packet.hpp"

class Router : public Node
{
public:
    explicit Router(IP _ip);
    ~Router();
    forward* createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID, int queueSize);
    void sendToPort(int portNum , QSharedPointer<Packet> pack);
    Port* getPort(int num);
    Port* getPort(IP ip);

private:
    QVector<route> routingTable;
    QVector<forward *> forwardingTable;
};

#endif // ROUTER_HPP
