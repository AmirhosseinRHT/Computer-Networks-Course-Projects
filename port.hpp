#ifndef PORT_HPP
#define PORT_HPP

#include <QObject>
#include <QQueue>
#include "packet.hpp"

class Port : public QObject
{
    Q_OBJECT
public:
    explicit Port(int maxq , QObject *parent = nullptr);
    void sendPacket(QSharedPointer<Packet> pack);
    void updateMaxQueueSize(int newSize);
    void increasePacketsInQueueCycle();
    bool isQueueEmpty(){return queue.isEmpty();}
    bool isQueueFull() {return queue.size() == maxQueueSize;}
    QSharedPointer<Packet> dequeue(){return queue.dequeue();}

private:
    QQueue<QSharedPointer<Packet>> queue;
    int maxQueueSize;

public slots:
    void recievePacket(QSharedPointer<Packet> pack);
signals:
    void newPacketArrived(QSharedPointer<Packet> pack);
};

#endif // PORT_HPP
