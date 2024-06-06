#include "host.hpp"
#include<QRandomGenerator>
#include<QTime>

QVector<QString> ip_list= {
    "192.170.1.2",
    "192.170.1.3",
    "192.170.2.2",
    "192.170.2.3",
    "192.170.3.2",
    "192.170.3.3",
    "192.170.4.2",
    "192.170.4.3",
    "200.200.3.2",
    "200.200.6.2",
    "200.200.9.2",
    "192.168.5.2",
    "192.168.5.3",
    "192.168.5.4",
    "192.168.6.2",
    "192.168.6.3"
};

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    ip = _ip;
    port = new Port(_portQueueSize);
    neighborRouter = "UNKNOWN";
    packetGeneratorDisttr = Parto(10 , 1);
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
    else{
        if(packetGeneratorDisttr.generate()){
            auto rg = QRandomGenerator(QTime::currentTime().msec());
            auto dest_ip = ip_list[rg.generate() % 16];
            while(getBaseIP(ip) == getBaseIP(dest_ip)){
                dest_ip = ip_list[rg.generate() % 16];
            }
            sendPacketTo(ip ,dest_ip);
        }
        handleIncomingPackets();
    }
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
        qDebug() << "start sending from " << src << " to " << dest << "\n";
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
