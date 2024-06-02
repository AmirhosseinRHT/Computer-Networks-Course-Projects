#include "cluster.hpp"

Cluster::Cluster(ClusterType _type, IP _baseIP, QObject *parent): QObject{parent}
{
    type = _type;
    baseIP = _baseIP;
}

Cluster::~Cluster()
{
    for(int i = 0 ; i < hosts.size() ; i++)
        delete hosts[i];
    for(int i = 0 ; i < threads.size() ;i++)
        delete threads[i];
}

void Cluster::createThreads(int n)
{
    for(int i = 0 ; i < n; i++)
        threads.append(new QThread);
}

void Cluster::connectPorts(Port * p1 , Port * p2)
{
    QObject::connect(p1, &Port::newPacketArrived, p2, &Port::recievePacket);
    QObject::connect(p2, &Port::newPacketArrived, p1, &Port::recievePacket);
}


void Cluster::connectRouters(Router * r1 , Router * r2)
{
    Port * p1 = r1->createForwardingRow("0" , "255" , "1" , r1->getPortQueueSize())->port;
    Port * p2 = r2->createForwardingRow("0" , "255" , "1" , r2->getPortQueueSize())->port;
    connectPorts(p1,p2);
}
void Cluster::connectRouterToHost(Router * r , Host * h)
{
    Port * p1 = r->createForwardingRow("0" , "255" , "1" , r->getPortQueueSize())->port;
    Port * p2 = h->getPort();
    connectPorts(p1,p2);
}

