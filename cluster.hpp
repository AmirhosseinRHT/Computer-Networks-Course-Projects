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
    explicit Cluster(ClusterType _Type  , IP _baseIP , QObject *parent = nullptr);
    ~Cluster();
    void createMeshCluster();
    QVector <Router *> getEdgeRouters();
    void connectPorts(Port * p1 , Port * p2);
    void connectRouters(Router * r1 , Router * r2);
    void connectRouterToHost(Router * r , Host * h);
    QVector<Host *> getHosts() {return hosts;}
    void createThreads(int n);
    IP getBaseIP(){return baseIP;}
private:
    ClusterType type;
    IP baseIP;

protected:
    QVector<QThread *> threads;
    QVector<Host *> hosts;
};

#endif // CLUSTER_H
