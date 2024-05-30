#ifndef NODE_HPP
#define NODE_HPP

#include <QObject>
#include <QDebug>
#include <QSharedPointer>
#include "defs.hpp"

class Node : public QObject
{
    Q_OBJECT
public:
    explicit Node(IP _ip,IPversion v ,int _portQueueSize,QObject *parent = nullptr);
    void createPacket();
    QVector<int> splitIp(QString ip);
    IPversion ver;
    int getPortQueueSize(){return portQueueSize;};
private:
    IP ip;
    int portQueueSize;

public slots:
    void onClock();
};

#endif // NODE_HPP
