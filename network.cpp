#include "network.hpp"
#include <QTime>
#include "cluster.hpp"

Network::Network(IP _ringStarBaseIP , IP _meshBaseIP ,IP _torusBaseIP , int _meshSize , int _torusSize, QObject * parent): QObject{parent}
{
    rsCluster= new RingStarCluster(RingStar , _ringStarBaseIP);
    mCluster= new MeshCluster(Mesh , _meshBaseIP , _meshSize);
    tCluster = new TorusCluster(Torus , _torusBaseIP , _torusSize);
    torusSize = _torusSize;
    meshSize = _meshSize;
    meshBaseIP = _meshBaseIP;
    ringStarBaseIP = _ringStarBaseIP;
    torusBaseIP = _torusBaseIP;
}

void Network::createNetwork()
{
    rsCluster->createRingStarCluster();
    mCluster->createMeshCluster();
    tCluster->createTorusCluster();
    connectEdgeRouters();
}

Network::~Network()
{
    delete rsCluster;
    delete mCluster;
    delete tCluster;
}

void Network::ConnectSignalsToNodes(Signaller * signaller , CommandReader * cr)
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
    {
        QObject::connect(signaller, &Signaller::Clock, rsrouters[i], &Router::onClock);
        QObject::connect(cr, &CommandReader::printRoutingTable, rsrouters[i], &Router::printTable);
    }

    QVector <QVector <Router *>>mrouters = mCluster->getRouters();
    for(int i = 0 ; i < mrouters.size() ; i ++ )
        for(int j = 0 ; j < mrouters[i].size() ; j++)
        {
            QObject::connect(signaller, &Signaller::Clock, mrouters[i][j], &Router::onClock);
            QObject::connect(cr, &CommandReader::printRoutingTable, mrouters[i][j], &Router::printTable);
        }

    QVector <QVector <Router *>>trouters = tCluster->getRouters();
    for(int i = 0 ; i < trouters.size() ; i ++ )
        for(int j = 0 ; j < trouters[i].size() ; j++)
        {
            QObject::connect(signaller, &Signaller::Clock, trouters[i][j], &Router::onClock);
            QObject::connect(cr, &CommandReader::printRoutingTable, trouters[i][j], &Router::printTable);
        }
}


void Network::connectEdgeRouters()
{
    QVector <Router *> tEdges = tCluster->getEdgeRouters();
    QVector <Router *> rsEdges = rsCluster->getEdgeRouters();
    QVector <Router *> mEdges = mCluster->getEdgeRouters();
    for(int i = 0 ; i < rsEdges.size() ; i++)
        Cluster::connectRouters(rsEdges[i] , mEdges[i]);
    for(int i = 0 ; i < tEdges.size() ; i++)
        Cluster::connectRouters(tEdges[i] , mEdges[i + rsEdges.size()]);
}

