#ifndef CLUSTER_H
#define CLUSTER_H

#include <QObject>
#include "defs.hpp"
#include "router.hpp"
#include "Host.hpp"

class Cluster : public QObject
{
    Q_OBJECT
public:
    explicit Cluster(ClusterType _Type  , QObject *parent = nullptr);
private:
    ClusterType type;
    QVector<Router *> routers;
    QVector<Host *> hosts;
};

#endif // CLUSTER_H
