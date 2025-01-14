#include "meshcluster.hpp"

MeshCluster::MeshCluster(ClusterType _type , IP _baseIP , int _n): Cluster{_type , _baseIP}
{
    n = _n;
}

QVector<Router*> MeshCluster::getEdgeRouters()
{
    QVector <Router *> edgeRouters;
    for (int i = 0; i < routers.size(); i++)
    {
        edgeRouters.append(routers[n-1][i]);
        routers[n-1][i]->isEdgeRouter = true;
    }
    for (int i = 0; i < routers.size(); i++)
    {
        routers[i][n-1]->isEdgeRouter = true;
        edgeRouters.append(routers[i][n-1]);
    }
    return edgeRouters;
}

MeshCluster::~MeshCluster()
{
    for(int i = 0 ; i < routers.size() ; i ++)
        for (int j = 0; j < routers[i].size(); j++)
            delete routers[i][j];
}


void MeshCluster::createMeshRoutersAndHosts() {
    for (int i = 0; i < n ; i++){
        QVector<Router*> temp;
        for (int j = 0; j < n ; j++){
            temp.append(new Router(getBaseIP() + "." + QString::number(j + i * n + 1) + ".1" , IPV4 , 100));
        }
        routers.append(temp);
    }
    for(int i = 0; i < 2 * n ; i++)
        hosts.append(new Host("NOTHING" , IPV4 , 10));
}

void MeshCluster::connectAllRouters() {
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

void MeshCluster::connectHostsToRouters()
{
    for(int j = 0 ; j < n; j++)
    {
        connectRouterToHost(routers[0][j] , hosts[2*j]);
        connectRouterToHost(routers[0][j] , hosts[2*j+1]);
    }

}

void MeshCluster::moveNodesToThread() {
    for (int i=0; i< n; i++)
        for (int j = 0 ; j < n; j++)
            routers[i][j]->moveToThread(threads[i*n + j]);
    for(int i=0 ; i < 2 * n ; i++)
        hosts[i]->moveToThread(threads[n*n + i]);
    for(int i = 0 ; i < threads.size() ; i ++ )
        threads[i]->start();
}

void MeshCluster::createMeshCluster()
{
    Cluster::createThreads(n *(n + 2));
    createMeshRoutersAndHosts();
    connectAllRouters();
    connectHostsToRouters();
    moveNodesToThread();
}





