#include "toruscluster.hpp"

TorusCluster::TorusCluster(ClusterType _type , IP _baseIP, int _n) : Cluster(_type , _baseIP)
{
    n= _n;
}

TorusCluster::~TorusCluster()
{
    for(int i = 0 ; i < routers.size() ; i ++)
        for (int j = 0; j < routers[i].size(); j++)
            delete routers[i][j];
}

QVector<Router *> TorusCluster::getEdgeRouters()
{
    QVector<Router * > edges;
    for(int j = 0 ; j < n; j++)
    {
        routers[j][0]->isEdgeRouter = true;
        edges.append(routers[j][0]);
    }
    return edges;
}

void TorusCluster::createTorusRoutersAndHosts() {
    for (int i = 0; i < n ; i++){
        QVector<Router*> temp;
        for (int j = 0; j < n ; j++){
            temp.append(new Router(getBaseIP() + "." + QString::number(j + i * n + 1) + ".1" , IPV4 , 100));
        }
        routers.append(temp);
    }
    for(int i = 0; i < n ; i++)
        hosts.append(new Host("nothing" , IPV4 , 10));
}

void TorusCluster::connectAllRouters() {
    for (int i=0; i< n; i++)
    {
        for (int j = 0 ; j < n; j++)
        {
            if(j - 1 >= 0)
                connectRouters(routers[i][j],routers[i][ j - 1]);
            if(i - 1 >= 0)
                connectRouters(routers[i][j],routers[i - 1][ j ]);
            if(j + 1 < n )
                connectRouters(routers[i][j],routers[i][ j + 1]);
            if(i + 1 < n)
                connectRouters(routers[i][j],routers[i + 1][ j ]);
        }
    }
}

void TorusCluster::connectHostsToRouters()
{
    for(int j = 0 ; j < n; j++)
        connectRouterToHost(routers[j][n-1] , hosts[j]);

}

void TorusCluster::moveNodesToThread() {
    for (int i=0; i< n; i++)
        for (int j = 0 ; j < n; j++)
            routers[i][j]->moveToThread(threads[i*n + j]);
    for(int i=0 ; i < n ; i++)
        hosts[i]->moveToThread(threads[n*n + i]);
    for(int i = 0 ; i < threads.size() ; i++)
        threads[i]->start();
}

void TorusCluster::connectEdgeRouters()
{
    for(int i = 0 ; i < n ; i++)
    {
        connectRouters(routers[0][i] , routers[n-1][i]);
        connectRouters(routers[i][0] , routers[i][n-1]);
    }
}

void TorusCluster::createTorusCluster()
{
    Cluster::createThreads(n *(n + 1));
    createTorusRoutersAndHosts();
    connectAllRouters();
    connectEdgeRouters();
    connectHostsToRouters();
    moveNodesToThread();
}
