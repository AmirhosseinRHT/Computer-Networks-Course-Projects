#include "controller.hpp"
#include <QTime>

Controller::Controller(IP _ringStartBaseIP , IP _meshBaseIP , int _MeshSize , QObject * parent): QObject{parent}
{
    rsCluster= new RingStarCluster(RingStar , _ringStartBaseIP);
    rsCluster->createRingStarCluster();
    MeshCluster * mCluster= new MeshCluster(Mesh , _meshBaseIP , _MeshSize);
    mCluster->createMeshCluster();
    meshSize = _MeshSize;
    meshBaseIP = _meshBaseIP;
    ringStarBaseIP = _ringStartBaseIP;
}


void Controller::main()
{
    while(true)
    {
        emit Clock();
        QThread::msleep(1000);
    }
}
