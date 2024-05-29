#ifndef CLUSTER_H
#define CLUSTER_H

#include <QObject>
#include <QThread>
#include "defs.hpp"
#include "router.hpp"
#include "Host.hpp"

class Cluster : public QObject
{
    Q_OBJECT
public:
    explicit Cluster(ClusterType _Type  , QObject *parent = nullptr);
    ~Cluster();
    void createMeshCluster();
    void createRingStarCluster();
    QVector <Router *> getEdgeRouters();
    void connectPorts(Port * p1 , Port * p2);

private:
    ClusterType type;
    QVector<Router *> routers;
    QVector<Host *> hosts;
    QVector<QThread *> threads;
};

#endif // CLUSTER_H
