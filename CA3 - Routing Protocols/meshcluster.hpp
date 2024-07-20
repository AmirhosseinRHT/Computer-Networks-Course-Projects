#ifndef MESHCLUSTER_HPP
#define MESHCLUSTER_HPP

#include <QObject>
#include "defs.hpp"
#include "cluster.hpp"

class MeshCluster : public Cluster
{
    Q_OBJECT
public:
    explicit MeshCluster(ClusterType _type , IP _baseIP , int _n);
    ~MeshCluster();
    QVector<Router*> getEdgeRouters();
    void createMeshCluster();
    void createMeshRoutersAndHosts();
    void moveNodesToThread();
    void connectAllRouters();
    void connectHostsToRouters();
    QVector<QVector<Router * >> getRouters(){return routers;}
protected:
    QVector<QVector<Router * >> routers;
    int n;
signals:
};

#endif // MESHCLUSTER_HPP
