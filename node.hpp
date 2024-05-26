#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include "defs.hpp"
#include "packet.hpp"

class node : public QObject
{
    Q_OBJECT
public:
    explicit node(IP _ip ,  QObject *parent = nullptr);
private:
    IP ip;

public slots:
    void getPacket(packet* pack);


signals:
    void sendPacket(packet * pack);



};

#endif // NODE_HPP
