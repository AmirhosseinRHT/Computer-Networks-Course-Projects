#include "cluster.hpp"

Cluster::Cluster(ClusterType _type, QObject *parent): QObject{parent}
{
    type = _type;
}


