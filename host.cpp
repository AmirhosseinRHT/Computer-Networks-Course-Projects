#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    port = new Port(_portQueueSize);
    neighborRouter = "";
}

Host::~Host()
{
    delete port;
}

void Host::onClock(NetworkState ns)
{
    currentState = ns;
    if(currentState == InteractionWithDHCP)
        getIpFromDHCPServer();
    else
    {
        handleIncomingPackets();
    }
}

void Host::getIpFromDHCPServer()
{
        Packet pack("0" , "0" , "IP_REQUEST" , DHCP);
        port->sendPacket(QSharedPointer<Packet>::create(pack));
}


void Host::handleIncomingPackets()
{
    while(!port->isQueueEmpty())
    {
        QSharedPointer<Packet> p = port->dequeue();
        if(p->getData() == "DHCP_ANSWER")
        {
            ip = p->getDestiantionAddr();
            neighborRouter = p->getSourceAddr();
        }
        else{}
    }
}
