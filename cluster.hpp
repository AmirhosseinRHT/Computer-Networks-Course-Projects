#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include <QObject>
#include "defs.hpp"

class cluster : public QObject
{
    Q_OBJECT
public:
    explicit cluster(ClusterType _Type  , QObject *parent = nullptr);

private:
    ClusterType type;

};

#endif // CLUSTER_HPP
