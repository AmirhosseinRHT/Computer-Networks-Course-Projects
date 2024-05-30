#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include "meshcluster.hpp"
#include "ringstarcluster.hpp"

class Controller : public QObject
{
    Q_OBJECT
public:
   explicit Controller(QObject *parent = nullptr);
    void main();
   MeshCluster * getMeshCluster(){return mCluster;}
   RingStarCluster * getRingStarCluster(){return rsCluster;}
private:
    MeshCluster * mCluster;
    RingStarCluster * rsCluster;
signals:
   void Pulse();
   void StartGreeting();
};

#endif // CONTROLLER_HPP
