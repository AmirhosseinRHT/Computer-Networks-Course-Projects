#include "node.hpp"

node::node(IP _ip , QObject *parent): QObject{parent}
{
    ip = _ip;
}

void node::send(QSharedPointer<packet> pack)
{
    emit sendPacket(pack);
}

void node::recievePacket(QSharedPointer<packet> pack)
{
    qDebug() << "got : " << pack->getPacket() << "\n";
}

void node::createPacket()
{
    packet pack("so", "dest", "hello world", Data);
    QSharedPointer<packet> temp = QSharedPointer<packet>::create(pack);
    emit sendPacket(temp);
}
