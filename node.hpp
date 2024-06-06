#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include "defs.hpp"
#include"parto.hpp"

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(IP _ip,IPversion v ,int _portQueueSize,QObject *parent = nullptr);
    QVector<int> splitIp(QString ip);
    IPversion ver;
    int getPortQueueSize(){return portQueueSize;}
    QVector<QString> splitPacket(QString str, char del);
    RoutingAlgorithm routeAlgo;
protected:
    NetworkState currentState;
    IP ip;
    int portQueueSize;

public slots:
    virtual void onClock(NetworkState ns){currentState = ns;}
};

#endif // NODE_HPP
