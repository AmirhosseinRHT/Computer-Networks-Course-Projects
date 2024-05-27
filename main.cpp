#include <QCoreApplication>
#include "router.hpp"
#include <qthread.h>
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QThread thread1 , thread2;

    // Create the Router objects
    router router1("test1");
    router router2("test2");

    // Move the Router objects to their respective threads
    router1.moveToThread(&thread1);
    router2.moveToThread(&thread2);

    // Connect the signals and slots
    QObject::connect(&router1, &router::sendPacket, &router2, &router::recievePacket);
    QObject::connect(&router2, &router::sendPacket, &router1, &router::recievePacket);

    // Start the threads
    thread1.start();
    thread2.start();

    router1.createPacket();
    return a.exec();
}



