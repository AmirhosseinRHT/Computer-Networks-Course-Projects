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
    QObject::connect(&(*p1), &Port::newPacketArrived, &(*p2), &Port::recievePacket);
    QObject::connect(&(*p2), &Port::newPacketArrived, &(*p1), &Port::recievePacket);
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


// void Cluster::createMeshCluster()
// {
//     for(int i = 0 ; i < 23; i++)
//         threads.append(new QThread);

//     for(int i = 0 ; i < 15 ; i++)
//     {
//         routers.append(new Router("192.168." +QString::number(i+1) + ".1" , IPV4));
//         routers[i]->createForwardingRow("0" , "255" , "1" , 10);
//         routers[i]->createForwardingRow("1" , "255" , "1" , 10);
//         if(i != 3)
//             routers[i]->createForwardingRow("2" , "255" , "1" , 10);
//         if(i%4 == 1 || i%4 ==2)
//             routers[i]->createForwardingRow("3" , "255" , "1" , 10);
//         if(i == 13 || i == 14)
//             routers[i]->createForwardingRow("4" , "255" , "1" , 10);
//         routers[i]->moveToThread(threads[i]);
//     }
//     for(int i = 0 ; i < 8 ; i++)
//     {
//         hosts.append(new Host(QString::number(i) , IPV4 , 10));
//         hosts[i]->moveToThread(threads[i+15]);
//     }

// }

