#include "router.hpp"

Router::Router(IP _ip) :Node(_ip) {}

Router::~Router()
{
    for(int i = 0 ; i < forwardingTable.size() ; i++)
    {
        delete forwardingTable[i]->port;
        delete forwardingTable[i];
    }
}

forward* Router::createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID , int queueSize)
{
    Port *p = new Port(queueSize);
    forward * f = new forward{hopID , subnetMask , subnetID , p};
    forwardingTable.append(f);
    return forwardingTable[0];
}

void Router::sendToPort(int portNum , QSharedPointer<Packet> pack)
{
    forwardingTable[portNum-1]->port->sendPacket(pack);
}

Port * Router::getPort(int num)
{
    if(num < forwardingTable.size())
        return forwardingTable[num]->port;
    return NULL;
}

Port * Router::getPort(IP ip)
{
    for(int i = 0; i < forwardingTable.size(); i++)
        if(forwardingTable[i]->nextHopIP == ip)
            return forwardingTable[i]->port;
    return NULL;
}


