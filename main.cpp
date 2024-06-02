#include <QCoreApplication>
#include "controller.hpp"
#include <QThread>
#include <QHostAddress>
#include <QString>
#include <QTextStream>

void moveRingStarNodesToThread(QVector <QThread * > RsThreads ,  RingStarCluster * rs)
{
    QVector <Router *> routers =rs->getRouters();
    QVector <Host *> hosts =rs->getHosts();
    for(int i = 0 ; i < 13 ; i ++)
        RsThreads.append(new QThread());
    for(int i = 0 ; i < 5 ; i++)
        hosts[i]->moveToThread(RsThreads[i+8]);
    for (int i = 0; i < 8; i++)
        routers[i]->moveToThread(RsThreads[i]);
    for(int i = 0 ; i < RsThreads.size() ; i ++ )
        RsThreads[i]->start();
}

void moveMeshNodesToThread(QVector <QThread * > mThreads ,  MeshCluster * m ,int n)
{
    QVector <QVector<Router *>> routers =m->getRouters();
    QVector <Host *> hosts =m->getHosts();
    for(int i = 0 ; i < n * (n + 2); i++)
        mThreads.append(new QThread);
    for (int i=0; i< n; i++)
        for (int j = 0 ; j < n; j++)
            routers[i][j]->moveToThread(mThreads[i*4 + j]);
    for(int i=0 ; i < 2 * n ; i++)
        hosts[i]->moveToThread(mThreads[n*n + i]);
    for(int i = 0 ; i < mThreads.size() ; i ++ )
    {
        mThreads[i]->start();
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Controller * controller = new Controller("192.168" , "192.170" , 4);
    Signaller * signaller = new Signaller();
    controller->ConnectClockToNodes(signaller);
    QThread th1;
    signaller->moveToThread(&th1);
    QVector <QThread * > RsThreads ,  mThreads;
    // moveRingStarNodesToThread(RsThreads , controller->getRingStarCluster());
    // moveMeshNodesToThread(mThreads , controller->getMeshCluster() , 4);
    signaller->main();


    // //////
    // // QVector <Host*>hosts = controller->getMeshCluster()->getHosts();

    // // QObject::connect((controller2), &Controller::Clock, hosts[0], &Host::onClock);
    // // controller->moveToThread(&t);
    // // hosts[0]->moveToThread(&t2);
    // // controller->ConnectClockToNodes();

    // QVector<Host *> hosts = controller->getMeshCluster()->getHosts();
    // for(int i = 0 ; i < hosts.size() ; i ++ )
    //     QObject::connect((controller2), &Controller::Clock, hosts[i], &Host::onClock);

    // hosts = controller->getRingStarCluster()->getHosts();
    // for(int i = 0 ; i < hosts.size() ; i ++ )
    //     QObject::connect((controller2), &Controller::Clock, hosts[i], &Host::onClock);

    // QVector <Router *> rsrouters = controller->getRingStarCluster()->getRouters();
    // for(int i = 0 ; i < rsrouters.size() ; i ++ )
    //     QObject::connect((controller2), &Controller::Clock, rsrouters[i], &Router::onClock);

    // QVector <QVector <Router *>>mrouters = controller->getMeshCluster()->getRouters();
    // for(int i = 0 ; i < mrouters.size() ; i ++ )
    //     for(int j = 0 ; j < mrouters[i].size() ; j++)
    //         QObject::connect((controller2), &Controller::Clock, mrouters[i][j], &Router::onClock);
    // // //////
    // controller->getMeshCluster()->moveNodesToThread();
    // controller->getRingStarCluster()->moveNodesToThread();
    // controller2->moveToThread(&t);
    // t.start();
    // controller2->main();


    // // Packet pack1("192.168.1.2" , "192.168.1.3" , "hello from 2 to 3" , Data);
    // // RingStarCluster * rsCluster= new RingStarCluster(RingStar , "192.168");
    // // rsCluster->createRingStarCluster();
    // // rsCluster->getRouters()[7]->getPort(0)->sendPacket(QSharedPointer<Packet>::create(pack1));

    // Packet pack1("192.168.100000.2" , "192.168.20000.3" , "hello from 2 to 3" , Data);
    // // MeshCluster * mCluster= new MeshCluster(Mesh , "192.168" , 4);
    // MeshCluster * mCluster = controller->getMeshCluster();
    // mCluster->createMeshCluster();
    // mCluster->getRouters()[2][2]->getPort(0)->sendPacket(QSharedPointer<Packet>::create(pack1));
    // delete controller;
    // QCoreApplication::quit();

    // QThread thread1 , thread2;

    // // Create the Router objects
    // Router router1("test1" , IPV4 , 10);
    // Router router2("test2" , IPV4 , 10);
    // Port * p1 = router1.createForwardingRow("1" , "255" , "0" , 10)->port;
    // Port * p2 = router2.createForwardingRow("2" , "255" , "0", 10)->port;
    // // Move the Router objects to their respective threads
    // router1.moveToThread(&thread1);
    // router2.moveToThread(&thread2);
    // // Connect the signals and slots
    // QObject::connect(&(*router1.getPort("1")), &Port::newPacketArrived, &(*router2.getPort("2")), &Port::recievePacket);
    // QObject::connect(&(*router2.getPort(0)), &Port::newPacketArrived, &(*router1.getPort(0)), &Port::recievePacket);

    // // Start the threads
    // // thread1.start();
    // // thread2.start();
    // /////////////////////////////////////////
    // Packet pack1("192.168.1.2" , "192.168.1.3" , "hello from 2 to 3" , Data);
    // Packet pack2("192.168.1.3" , "192.168.1.2" , "hello from 3 to 2" , Data);
    // (*p1).sendPacket(QSharedPointer<Packet>::create(pack1));
    // (*p2).sendPacket(QSharedPointer<Packet>::create(pack2));

    return a.exec();
}



