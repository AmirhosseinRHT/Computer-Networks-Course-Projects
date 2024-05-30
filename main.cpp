#include <QCoreApplication>
// #include "ringstarcluster.hpp"
// #include "meshcluster.hpp"
#include "controller.hpp"
#include <QThread>

#include <QHostAddress>
#include <QString>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Controller * controller = new Controller("192.168" , "192.170" , 4);
    delete controller;
    // for(int i = 0 ; i < 10; i ++)
    // {
    //     Packet pack1("192.168.1.2" , "192.168.1.3" , "hello from 2 to 3" , Data);
    //     RingStarCluster * rsCluster= new RingStarCluster(RingStar , "192.168");
    //     rsCluster->createRingStarCluster();
    //     rsCluster->getRouters()[7]->getPort(0)->sendPacket(QSharedPointer<Packet>::create(pack1));
    // }

    // for(int i = 0 ; i < 10; i ++)
    // {
    //     Packet pack1("192.168.100000.2" , "192.168.20000.3" , "hello from 2 to 3" , Data);
    //     MeshCluster * mCluster= new MeshCluster(Mesh , "192.168" , 4);
    //     mCluster->createMeshCluster();
    //     mCluster->getRouters()[2][2]->getPort(0)->sendPacket(QSharedPointer<Packet>::create(pack1));
    // }

    QCoreApplication::quit();

    // QThread thread1 , thread2;

    // // Create the Router objects
    // Router router1("test1" , IPV4);
    // Router router2("test2" , IPV4);
    // Port * p1 = router1.createForwardingRow("1" , "255" , "0" , 10)->port;
    // Port * p2 = router2.createForwardingRow("2" , "255" , "0", 10)->port;
    // // Move the Router objects to their respective threads
    // router1.moveToThread(&thread1);
    // router2.moveToThread(&thread2);
    // // Connect the signals and slots
    // QObject::connect(&(*router1.getPort("1")), &Port::newPacketArrived, &(*router2.getPort("2")), &Port::recievePacket);
    // QObject::connect(&(*router2.getPort(0)), &Port::newPacketArrived, &(*router1.getPort(0)), &Port::recievePacket);

    // // Start the threads
    // thread1.start();
    // thread2.start();
    // /////////////////////////////////////////
    // Packet pack1("192.168.1.2" , "192.168.1.3" , "hello from 2 to 3" , Data);
    // Packet pack2("192.168.1.3" , "192.168.1.2" , "hello from 3 to 2" , Data);
    // (*p1).sendPacket(QSharedPointer<Packet>::create(pack1));
    // (*p2).sendPacket(QSharedPointer<Packet>::create(pack2));

    return a.exec();
}



