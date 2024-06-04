#include "controller.hpp"
#include <QTime>

Controller::Controller(IP _ringStarBaseIP , IP _meshBaseIP ,IP _torusBaseIP , int _meshSize , int _torusSize, QObject * parent): QObject{parent}
{
    rsCluster= new RingStarCluster(RingStar , _ringStarBaseIP);
    rsCluster->createRingStarCluster();
    mCluster= new MeshCluster(Mesh , _meshBaseIP , _meshSize);
    mCluster->createMeshCluster();
    tCluster = new TorusCluster(Torus , _torusBaseIP , _torusSize);
    tCluster->createTorusCluster();
    torusSize = _torusSize;
    meshSize = _meshSize;
    meshBaseIP = _meshBaseIP;
    ringStarBaseIP = _ringStarBaseIP;
    torusBaseIP = _torusBaseIP;
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

    hosts = tCluster->getHosts();
    for(int i = 0 ; i < hosts.size() ; i ++ )
        QObject::connect(signaller, &Signaller::Clock, hosts[i], &Host::onClock);

    QVector <Router *> rsrouters = rsCluster->getRouters();
    for(int i = 0 ; i < rsrouters.size() ; i ++ )
        QObject::connect(signaller, &Signaller::Clock, rsrouters[i], &Router::onClock);

    QVector <QVector <Router *>>mrouters = mCluster->getRouters();
    for(int i = 0 ; i < mrouters.size() ; i ++ )
        for(int j = 0 ; j < mrouters[i].size() ; j++)
            QObject::connect(signaller, &Signaller::Clock, mrouters[i][j], &Router::onClock);

    QVector <QVector <Router *>>trouters = tCluster->getRouters();
    for(int i = 0 ; i < trouters.size() ; i ++ )
        for(int j = 0 ; j < trouters[i].size() ; j++)
            QObject::connect(signaller, &Signaller::Clock, trouters[i][j], &Router::onClock);

    // mCluster->moveNodesToThread();
    // rsCluster->moveNodesToThread();
}

