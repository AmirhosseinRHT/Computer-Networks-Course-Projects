#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include "meshcluster.hpp"
#include "ringstarcluster.hpp"
#include "signaller.hpp"
#include "toruscluster.hpp"
#include "commandreader.hpp"

class Network : public QObject
{
    Q_OBJECT
public:
    explicit Network(IP _ringStarBaseIP , IP _meshBaseIP ,IP _torusBaseIP , int _MeshSize , int TorusSize , QObject * parent = nullptr);
    ~Network();
   MeshCluster * getMeshCluster(){return mCluster;}
   RingStarCluster * getRingStarCluster(){return rsCluster;}
   TorusCluster * getTorusCluster(){return tCluster;}
   void ConnectSignalsToNodes(Signaller * signaller , CommandReader * cr);
   void createNetwork();
   void connectEdgeRouters();
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

#endif // NETWORK_H
