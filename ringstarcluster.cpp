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

void RingStarCluster::moveNodesToThread()
{
    for(int i = 0 ; i < 5 ; i++)
        hosts[i]->moveToThread(this->thread());
    for (int i = 0; i < 8; i++)
        routers[i]->moveToThread(this->thread());
    for(int i = 0 ; i < threads.size() ; i ++ )
    {
        // threads[i]->start();
    }
}

void RingStarCluster::createRoutersAndHosts()
{
    for(int i = 0 ; i < 8 ; i++)
        routers.append(new Router(getBaseIP() + "." + QString::number(i+1) + ".1" , IPV4 , 10));
    for(int i = 0 ; i < 5 ; i++)
        hosts.append(new Host("nothing" , IPV4 , 10));
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

    connectRouterToHost(routers[4] , hosts[3]);
    connectRouterToHost(routers[4] , hosts[4]);
}

void RingStarCluster::createRingStarCluster()
{
    Cluster::createThreads(13);
    createRoutersAndHosts();
    connectRingStarPorts();
    moveNodesToThread();
}
