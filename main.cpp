#include <QCoreApplication>
#include "router.hpp"
#include <QThread>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QThread thread1 , thread2;

    // Create the Router objects
    Router router1("test1");
    Router router2("test2");
    Port * p1 = router1.createForwardingRow("1" , "255" , "0" , 10)->port;
    Port * p2 = router2.createForwardingRow("2" , "255" , "0", 10)->port;
    // Move the Router objects to their respective threads
    router1.moveToThread(&thread1);
    router2.moveToThread(&thread2);
    // Connect the signals and slots
    QObject::connect(&(*router1.getPort("1")), &Port::newPacketArrived, &(*router2.getPort("2")), &Port::recievePacket);
    QObject::connect(&(*router2.getPort(0)), &Port::newPacketArrived, &(*router1.getPort(0)), &Port::recievePacket);

    // Start the threads
    thread1.start();
    thread2.start();
    Packet pack1("192.168.1.2" , "192.168.1.3" , "hello from 2 to 3" , Data);
    Packet pack2("192.168.1.3" , "192.168.1.2" , "hello from 3 to 2" , Data);
    (*p1).sendPacket(QSharedPointer<Packet>::create(pack1));
    (*p2).sendPacket(QSharedPointer<Packet>::create(pack2));
    return a.exec();
}



