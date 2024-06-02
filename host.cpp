#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    port = new Port(_portQueueSize);
}

Host::~Host()
{
    delete port;
}

void Host::onClock()
{
    qDebug() << "wdfnhiserojopd\n";
    getIpFromDHCPServer();
}

void Host::getIpFromDHCPServer()
{
        Packet pack("0" , "0" , "IP_REQUEST" , DHCP);
        port->sendPacket(QSharedPointer<Packet>::create(pack));
}


