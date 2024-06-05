#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include "meshcluster.hpp"
#include "ringstarcluster.hpp"
#include "signaller.hpp"
#include "toruscluster.hpp"

class Controller : public QObject
{
    Q_OBJECT
public:
   explicit Controller(IP _ringStarBaseIP , IP _meshBaseIP ,IP _torusBaseIP , int _MeshSize , int TorusSize , QObject * parent = nullptr);
   ~Controller();
   MeshCluster * getMeshCluster(){return mCluster;}
   RingStarCluster * getRingStarCluster(){return rsCluster;}
   TorusCluster * getTorusCluster(){return tCluster;}
   void ConnectClockToNodes(Signaller * signaller);
private:
    MeshCluster * mCluster;
    RingStarCluster * rsCluster;
    TorusCluster * tCluster;
    IP ringStarBaseIP;
    IP meshBaseIP;
    IP torusBaseIP;
    int meshSize;
    int torusSize;
signals:
   void Clock();
};

#endif // CONTROLLER_HPP
