#include "node.hpp"
#include "packet.hpp"

Node::Node(IP _ip ,IPversion v  , int _portQueueSize, QObject *parent): QObject{parent}
{
    ip = getCompatibleIP(_ip , v);
    ver = v;
    portQueueSize = _portQueueSize;
    routeAlgo = RIP;
}

QVector<QString> Node::splitPacket(QString str, char del)
{
    QVector<QString> resulte_vector;
    QString temp = "";
    for(int i=0; i<(int)str.size(); i++){
        if(str[i] != del){
            temp += str[i];
        }
        else{
            if(temp.size() > 0)
                resulte_vector.push_back(temp);
            temp = "";
        }
    }
    if(temp.size() > 0)
        resulte_vector.push_back(temp);
    return resulte_vector;
}

QVector<int> Node::splitIp(QString ip)
{
    QStringList parts = ip.split('.');
    QVector<int> result;

    for (const QString& part : parts)
    {
        bool ok;
        int num = part.toInt(&ok);
        if (ok)
            result.append(num);
    }
    return result;
}

