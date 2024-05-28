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
    explicit Node(IP _ip, QObject *parent = nullptr);
    void createPacket();
    QVector<int> splitIp(QString ip);
private:
    IP ip;

public slots:
    // void onPulse();
};

#endif // NODE_HPP
