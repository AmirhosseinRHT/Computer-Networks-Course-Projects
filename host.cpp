#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    port = new Port(_portQueueSize);
    neighborRouter = "UNKNOWN";
}

Host::~Host()
{
    delete port;
}

QSharedPointer<Packet> Host::createPacket(IP destination , QString data)
{
    Packet pack(ip , destination , data , Data);
    return QSharedPointer<Packet>::create(pack);
}

void Host::onClock(NetworkState ns)
{
    currentState = ns;
    if(currentState == InteractionWithDHCP)
        getIpFromDHCPServer();
    else
        handleIncomingPackets();
}

void Host::getIpFromDHCPServer()
{
        Packet pack(ip , neighborRouter , "IP_REQUEST" , DHCP);
        port->sendPacket(QSharedPointer<Packet>::create(pack));
}


void Host::sendPacketTo(QString src , QString dest)
{
    if(src == ip)
    {
        Packet pack(ip , dest , "!!!!!Hello!!!!!" , Data);
        port->sendPacket(QSharedPointer<Packet>::create(pack));
    }
}

void Host::handleIncomingPackets()
{
    while(!port->isQueueEmpty())
    {
        QSharedPointer<Packet> p = port->dequeue();
        if(p->getData() == "DHCP_ANSWER" && p->getType() == DHCP)
        {
            ip = p->getDestiantionAddr();
            neighborRouter = p->getSourceAddr();
        }
        else if(p->getType() == Data)
        {
            qDebug() << "------------------------------------------------------------------";
            qDebug() << "Host " + p->getDestiantionAddr() + " Recieved Packet from " + p->getSourceAddr()
                            + " data : " + p->getData();
            p->printLogs();
            qDebug() << "------------------------------------------------------------------";
        }
    }
}
