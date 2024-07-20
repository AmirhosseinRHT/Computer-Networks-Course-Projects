#include "port.hpp"

Port::Port(int maxq , QObject *parent): QObject{parent}
{
    maxQueueSize = maxq;
}

void Port::sendPacket(QSharedPointer<Packet> pack)
{
    emit newPacketArrived(pack);
}

void Port::updateMaxQueueSize(int newSize)
{
    if(newSize > maxQueueSize)
        maxQueueSize = newSize;
}

void Port::recievePacket(QSharedPointer<Packet> pack)
{
    // qDebug() << "Recieved Packet : "<<  pack->getPacket() << "\n";
    if(maxQueueSize == queue.size())
        qDebug() << "packet Dropped due to queue owerflowing!\n" ;
    else
        queue.enqueue(pack);
    return;
}

void Port::increasePacketsInQueueCycle()
{
    for(auto it = queue.cbegin();it!=queue.cend();it++)
        (*it)->icreaseInQueueCycle();
}
