#include "cluster.hpp"

cluster::cluster(ClusterType _type, QObject *parent): QObject{parent}
{
    type = _type;
}


