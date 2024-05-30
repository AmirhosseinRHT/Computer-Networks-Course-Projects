#ifndef RINGSTARCLUSTER_HPP
#define RINGSTARCLUSTER_HPP

#include <QObject>
#include "cluster.hpp"


class RingStarCluster : public Cluster
{
    Q_OBJECT
public:
    explicit RingStarCluster(ClusterType _type , IP _baseIP);
    ~RingStarCluster();
    void createRingStarCluster();
    QVector <Router *> getEdgeRouters();
    QVector <Router *> getRouters() {return routers;};
    void moveHostsToThread();
    void createThreads();
    void createRoutersAndPorts();
    void connectRingStarPorts();

private:
    QVector<Router *> routers;
signals:
};

#endif // RINGSTARCLUSTER_HPP
