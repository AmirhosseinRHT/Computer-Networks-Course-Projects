#include "cluster.hpp"

Cluster::Cluster(ClusterType _type, QObject *parent): QObject{parent}
{
    type = _type;
}

Cluster::~Cluster()
{
    for(int i = 0 ; i < routers.size() ;i++)
        delete routers[i];
    for(int i = 0 ; i < hosts.size() ; i++)
        delete hosts[i];
    for(int i = 0 ; i < threads.size() ;i++)
        delete threads[i];
}

QVector <Router *> Cluster::getEdgeRouters()
{
    QVector<Router *> temp;
    if(routers.size() >= 3)
        temp = QVector<Router *> (routers.begin() , routers.begin()+3);
    return temp;
}

void Cluster::connectPorts(Port * p1 , Port * p2)
{
    QObject::connect(&(*p1), &Port::newPacketArrived, &(*p2), &Port::recievePacket);
    QObject::connect(&(*p2), &Port::newPacketArrived, &(*p1), &Port::recievePacket);
}

void Cluster::createRingStarCluster()
{
    for(int i = 0 ; i < 13; i++)
        threads.append(new QThread);

    for(int i = 0 ; i < 8 ; i++)
    {
        routers.append(new Router("192.168." +QString::number(i+1) + ".1" , IPV4));
        routers[i]->createForwardingRow("0" , "255" , "1" , 10);
        routers[i]->createForwardingRow("1" , "255" , "1" , 10);
        routers[i]->moveToThread(threads[i]);
    }
    for(int i = 0 ; i < 5 ; i++)
    {
        hosts.append(new Host(QString::number(i) , IPV4 , 10));
        hosts[i]->moveToThread(threads[i+8]);
    }

    for(int i = 0 ; i < 7 ; i++)
    { // connect ring routers
        connectPorts(routers[i]->getPort(1) , routers[(i+6)%7]->getPort(0));
    }
    //add star ports and create center router
    routers[0]->createForwardingRow("2" , "255" , "1" , 10);
    routers[2]->createForwardingRow("2" , "255" , "1" , 10);
    routers[6]->createForwardingRow("2" , "255" , "1" , 10);
    routers[7]->createForwardingRow("2" , "255" , "1" , 10);
    routers[7]->createForwardingRow("3" , "255" , "1" , 10);

    routers[4]->createForwardingRow("2" , "255" , "1" , 10);
    routers[4]->createForwardingRow("3" , "255" , "1" , 10);
    routers[4]->createForwardingRow("4" , "255" , "1" , 10);

    routers[5]->createForwardingRow("2" , "255" , "1" , 10);
    routers[5]->createForwardingRow("3" , "255" , "1" , 10);
    routers[5]->createForwardingRow("4" , "255" , "1" , 10);

    connectPorts(routers[7]->getPort(0) , routers[0]->getPort(2));
    connectPorts(routers[7]->getPort(1) , routers[2]->getPort(2));
    connectPorts(routers[7]->getPort(2) , routers[4]->getPort(2));
    connectPorts(routers[7]->getPort(3) , routers[6]->getPort(2));

    connectPorts(routers[5]->getPort(2) , hosts[0]->getPort());
    connectPorts(routers[5]->getPort(3) , hosts[1]->getPort());
    connectPorts(routers[5]->getPort(4) , hosts[2]->getPort());

    connectPorts(routers[4]->getPort(3) , hosts[3]->getPort());
    connectPorts(routers[4]->getPort(4) , hosts[4]->getPort());

}


void Cluster::createMeshCluster()
{
    for(int i = 0 ; i < 23; i++)
        threads.append(new QThread);

    for(int i = 0 ; i < 15 ; i++)
    {
        routers.append(new Router("192.168." +QString::number(i+1) + ".1" , IPV4));
        routers[i]->createForwardingRow("0" , "255" , "1" , 10);
        routers[i]->createForwardingRow("1" , "255" , "1" , 10);
        if(i != 3)
            routers[i]->createForwardingRow("2" , "255" , "1" , 10);
        if(i%4 == 1 || i%4 ==2)
            routers[i]->createForwardingRow("3" , "255" , "1" , 10);
        if(i == 13 || i == 14)
            routers[i]->createForwardingRow("4" , "255" , "1" , 10);
        routers[i]->moveToThread(threads[i]);
    }
    for(int i = 0 ; i < 8 ; i++)
    {
        hosts.append(new Host(QString::number(i) , IPV4 , 10));
        hosts[i]->moveToThread(threads[i+15]);
    }

}

