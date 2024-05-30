#include "ringstarcluster.hpp"
#include "defs.hpp"

RingStarCluster::RingStarCluster(ClusterType _type , IP _baseIP): Cluster{_type , _baseIP}
{}

QVector <Router *> RingStarCluster::getEdgeRouters()
{
    QVector<Router *> temp;
    if(routers.size() >= 3)
        temp = QVector<Router *> (routers.begin() , routers.begin()+3);
    return temp;
}

RingStarCluster::~RingStarCluster()
{
    for (int i = 0; i < routers.size(); i++)
        delete routers[i];
}

void RingStarCluster::moveHostsToThread()
{
    for(int i = 0 ; i < 5 ; i++)
    {
        hosts.append(new Host(QString::number(i) , IPV4 , 10));
        hosts[i]->moveToThread(threads[i+8]);
    }
}

void RingStarCluster::createRoutersAndPorts()
{
    for(int i = 0 ; i < 8 ; i++)
    {
        routers.append(new Router(getBaseIP() +QString::number(i+1) + ".1" , IPV4 , 10));
        routers[i]->moveToThread(threads[i]);
    }
    // routers[0]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[2]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[6]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[7]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[7]->createForwardingRow("3" , "255" , "1" , 10);

    // routers[4]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[4]->createForwardingRow("3" , "255" , "1" , 10);
    // routers[4]->createForwardingRow("4" , "255" , "1" , 10);

    // routers[5]->createForwardingRow("2" , "255" , "1" , 10);
    // routers[5]->createForwardingRow("3" , "255" , "1" , 10);
    // routers[5]->createForwardingRow("4" , "255" , "1" , 10);
}

void RingStarCluster::connectRingStarPorts()
{
    for(int i = 0 ; i < 7 ; i++)
    {
        connectRouters(routers[i] , routers[(i+6)%7]);
        // connectPorts(routers[i]->getPort(1) , routers[(i+6)%7]->getPort(0));
    }
    //add star ports and create center router
    for (int i = 0; i < 4; i++)
    {
        connectRouters(routers[7] , routers[i*2]);
        // connectPorts(routers[7]->getPort(i) , routers[i*2]->getPort(2));
    }

    for (int i = 0; i < 3; i++)
    {
        connectRouterToHost(routers[4] , hosts[i]);
        // connectPorts(routers[4]->getPort(i+2) , hosts[i]->getPort());
    }

    connectRouterToHost(routers[4] , hosts[3]);
    connectRouterToHost(routers[4] , hosts[4]);
    // connectPorts(routers[4]->getPort(3) , hosts[3]->getPort());
    // connectPorts(routers[4]->getPort(4) , hosts[4]->getPort());
}

void RingStarCluster::createRingStarCluster()
{
    Cluster::createThreads(13);
    createRoutersAndPorts();
    moveHostsToThread();
    connectRingStarPorts();
}
