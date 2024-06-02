#include "controller.hpp"
#include <QTime>

Controller::Controller(IP _ringStartBaseIP , IP _meshBaseIP , int _MeshSize , QObject * parent): QObject{parent}
{
    rsCluster= new RingStarCluster(RingStar , _ringStartBaseIP);
    rsCluster->createRingStarCluster();
    mCluster= new MeshCluster(Mesh , _meshBaseIP , _MeshSize);
    mCluster->createMeshCluster();
    meshSize = _MeshSize;
    meshBaseIP = _meshBaseIP;
    ringStarBaseIP = _ringStartBaseIP;
}

Controller::~Controller()
{
    delete rsCluster;
    delete mCluster;
}


void Controller::ConnectClockToNodes(Signaller * signaller)
{
    QVector <Host*>hosts = mCluster->getHosts();
    for(int i = 0 ; i < hosts.size() ; i ++ )
    QObject::connect(signaller, &Signaller::Clock, hosts[i], &Host::onClock);
    hosts = rsCluster->getHosts();
    for(int i = 0 ; i < hosts.size() ; i ++ )
        QObject::connect(signaller, &Signaller::Clock, hosts[i], &Host::onClock);
    QVector <Router *> rsrouters = rsCluster->getRouters();
    for(int i = 0 ; i < rsrouters.size() ; i ++ )
        QObject::connect(signaller, &Signaller::Clock, rsrouters[i], &Router::onClock);
    QVector <QVector <Router *>>mrouters = mCluster->getRouters();
    for(int i = 0 ; i < mrouters.size() ; i ++ )
        for(int j = 0 ; j < mrouters[i].size() ; j++)
            QObject::connect(signaller, &Signaller::Clock, mrouters[i][j], &Router::onClock);
    // mCluster->moveNodesToThread();
    // rsCluster->moveNodesToThread();
}


void Controller::main()
{
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock();
        QThread::msleep(5000);
    }
}
