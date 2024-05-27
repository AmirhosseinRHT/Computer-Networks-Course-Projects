#include "node.hpp"

Node::Node(IP _ip , QObject *parent): QObject{parent}
{
    ip = _ip;
}

// void node::send(QSharedPointer<packet> pack)
// {
//     emit sendPacket(pack);
// }

// void node::recievePacket(QSharedPointer<packet> pack)
// {
//     qDebug() << "got : " << pack->getPacket() << "\n";
// }

void Node::createPacket()
{
    Packet pack("so" , "dest" , "hello world" , Data);
    QSharedPointer<Packet> temp = QSharedPointer<Packet>::create(pack);

}
