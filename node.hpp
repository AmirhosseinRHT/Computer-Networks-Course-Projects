#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include <bits/unique_ptr.h>
#include "defs.hpp"
#include "packet.hpp"

class node : public QObject
{
    Q_OBJECT
public:
    explicit node(IP _ip, QObject *parent = nullptr);
    void send(QSharedPointer<packet> pack);
    void createPacket();
private:
    IP ip;

public slots:
    void recievePacket(QSharedPointer<packet> pack);
signals:
    void sendPacket(QSharedPointer<packet> pack);
};

#endif // NODE_HPP
