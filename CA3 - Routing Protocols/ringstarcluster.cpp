#include "ringstarcluster.hpp"
#include "defs.hpp"

RingStarCluster::RingStarCluster(ClusterType _type , IP _baseIP): Cluster{_type , _baseIP}
{}

QVector <Router *> RingStarCluster::getEdgeRouters()
{
    QVector<Router *> edges;
    if(routers.size() >= 3)
    {
        for(int i = 0 ; i < 3 ;i++)
            routers[i]->isEdgeRouter = true;
        edges = QVector<Router *> (routers.begin() , routers.begin()+3);
    }
    return edges;
}

RingStarCluster::~RingStarCluster()
{
    for (int i = 0; i < routers.size(); i++)
        delete routers[i];
}

void RingStarCluster::moveNodesToThread()
{
    for(int i = 0 ; i < 5 ; i++)
        hosts[i]->moveToThread(threads[i]);
    for (int i = 0; i < 8; i++)
        routers[i]->moveToThread(threads[i+5]);
    for(int i = 0 ; i < threads.size() ; i ++ )
    {
        threads[i]->start();
    }
}

void RingStarCluster::createRoutersAndHosts()
{
    for(int i = 0 ; i < 8 ; i++)
        routers.append(new Router(getBaseIP() + "." + QString::number(i+1) + ".1" , IPV4 , 100));
    for(int i = 0 ; i < 5 ; i++)
        hosts.append(new Host("NOTHING" , IPV4 , 10));
}

void RingStarCluster::connectRingStarPorts()
{
    for(int i = 0 ; i < 7 ; i++)
        connectRouters(routers[i] , routers[(i+6)%7]);
    //add center router
    for (int i = 0; i < 4; i++)
        connectRouters(routers[7] , routers[i*2]);

    for (int i = 0; i < 3; i++)
        connectRouterToHost(routers[4] , hosts[i]);

    connectRouterToHost(routers[5] , hosts[3]);
    connectRouterToHost(routers[5] , hosts[4]);
}

void RingStarCluster::createRingStarCluster()
{
    Cluster::createThreads(13);
    createRoutersAndHosts();
    connectRingStarPorts();
    moveNodesToThread();
}
