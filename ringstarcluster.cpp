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
        routers.append(new Router(getBaseIP() + "." + QString::number(i+1) + ".1" , IPV4 , 10));
        routers[i]->moveToThread(threads[i]);
    }
}

void RingStarCluster::connectRingStarPorts()
{
    for(int i = 0 ; i < 7 ; i++)
    {
        connectRouters(routers[i] , routers[(i+6)%7]);
    }
    //add center router
    for (int i = 0; i < 4; i++)
    {
        connectRouters(routers[7] , routers[i*2]);
    }

    for (int i = 0; i < 3; i++)
    {
        connectRouterToHost(routers[4] , hosts[i]);
    }

    connectRouterToHost(routers[4] , hosts[3]);
    connectRouterToHost(routers[4] , hosts[4]);
}

void RingStarCluster::createRingStarCluster()
{
    Cluster::createThreads(13);
    createRoutersAndPorts();
    moveHostsToThread();
    connectRingStarPorts();
}
