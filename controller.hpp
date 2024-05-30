#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include "meshcluster.hpp"
#include "ringstarcluster.hpp"

class Controller : public QObject
{
    Q_OBJECT
public:
   explicit Controller(IP _ringStartBaseIP , IP _meshBaseIP , int _MeshSize , QObject * parent = nullptr);
    void main();
   MeshCluster * getMeshCluster(){return mCluster;}
   RingStarCluster * getRingStarCluster(){return rsCluster;}
private:
    MeshCluster * mCluster;
    RingStarCluster * rsCluster;
    IP ringStarBaseIP;
    IP meshBaseIP;
    int meshSize;
signals:
   void Clock();
};

#endif // CONTROLLER_HPP
