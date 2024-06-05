#include <QCoreApplication>
#include "network.hpp"
#include "commandreader.hpp"
#include <QThread>
#include <QHostAddress>
#include <QString>
#include <QtConcurrent>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Network * net = new Network("192.168" , "192.170"  , "200.200", 4 , 3);
    Signaller * signaller = new Signaller(15000);
    CommandReader * cr = new CommandReader();
    QThread th1 , th2 , th3;
    signaller->moveToThread(&th1);
    net->moveToThread(&th2);
    cr->moveToThread(&th3);
    th1.start();
    th2.start();
    th3.start();
    net->createNetwork();
    net->ConnectSignalsToNodes(signaller , cr);
    auto temp1 = QtConcurrent::run(&CommandReader::readCommands, cr);
    auto temp2 = QtConcurrent::run(&Signaller::main, signaller);
    return a.exec();
}
