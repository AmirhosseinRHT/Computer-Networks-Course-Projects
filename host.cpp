#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    ip = _ip;
    port = new Port(_portQueueSize);
    neighborRouter = "UNKNOWN";
}

Host::~Host()
{
    delete port;
}

QSharedPointer<Packet> Host::createPacket(IP destination , QString data)
{
    Packet pack(ip , getCompatibleIP(destination , ver) , data , Data , ver);
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
        Packet pack(ip , neighborRouter , "IP_REQUEST" , DHCP , ver);
        port->sendPacket(QSharedPointer<Packet>::create(pack));
}


void Host::sendPacketTo(QString src , QString dest)
{
    if(ip == getCompatibleIP(src , ver))
    {
        Packet pack(ip , getCompatibleIP(dest , ver) , "!!!!!Hello!!!!!" , Data , ver);
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
            ip = getCompatibleIP(p->getDestiantionAddr() , ver);
            neighborRouter = getCompatibleIP(p->getSourceAddr() , ver);
        }
        else if(p->getType() == Data)
        {
            QSharedPointer<Packet> recieved = p;
            if(p->maskedPacket)
                recieved = p->getInnerPacket();
            qDebug() << "------------------------------------------------------------------";
            qDebug() << "Host " + recieved->getDestiantionAddr() + " Recieved Packet from " + recieved->getSourceAddr()
                            + " data : " + recieved->getData();
            recieved->printLogs();
            qDebug() << "------------------------------------------------------------------";
        }
    }
}
