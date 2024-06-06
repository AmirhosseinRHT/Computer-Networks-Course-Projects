#ifndef TORUSCLUSTER_HPP
#define TORUSCLUSTER_HPP
#include "cluster.hpp"

class TorusCluster  : public Cluster
{
    Q_OBJECT
public:
    TorusCluster(ClusterType _type , IP _baseIP, int _n);
    ~TorusCluster();
    void connectHostsToRouters();
    void connectEdgeRouters();
    void createTorusCluster();
    void createTorusRoutersAndHosts();
    void moveNodesToThread();
    void connectAllRouters();
    QVector<Router*> getEdgeRouters();
    QVector<QVector<Router * >> getRouters(){return routers;}
private:
    QVector<QVector<Router * >> routers;
    int n;
};

#endif // TORUSCLUSTER_HPP
