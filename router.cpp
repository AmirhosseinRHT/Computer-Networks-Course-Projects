#include "router.hpp"

Router::Router(IP _ip,IPversion v) :Node(_ip ,v) {}

Router::~Router()
{
    for(int i = 0 ; i < forwardingTable.size() ; i++)
    {
        delete forwardingTable[i]->port;
        delete forwardingTable[i];
    }
}

forward* Router::createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID , int queueSize)
{
    Port *p = new Port(queueSize);
    forward * f = new forward{hopID , subnetMask , subnetID , p};
    forwardingTable.append(f);
    return forwardingTable[0];
}

void Router::sendToPort(int portNum , QSharedPointer<Packet> pack)
{
    forwardingTable[portNum-1]->port->sendPacket(pack);
}

Port * Router::getPort(int num)
{
    if(num < forwardingTable.size())
        return forwardingTable[num]->port;
    return NULL;
}

Port * Router::getPort(IP ip)
{
    for(int i = 0; i < forwardingTable.size(); i++)
        if(forwardingTable[i]->nextHopIP == ip)
            return forwardingTable[i]->port;
    return NULL;
}

IP Router::requestIP(int portNum)
{
    for(int i = 0 ; i < assignedIPs.size(); i++)
        if(assignedIPs[i].first == portNum)
            return assignedIPs[i].second;
    QVector<int> temp = splitIp(assignedIPs[assignedIPs.size()-1].second);
    if(temp[3] >= 255)
        return NULL; //no capacity
    temp[3]++;
    QString newIP = QString::number(temp[0])+"."+QString::number(temp[1])+"."+QString::number(temp[2])+"."+QString::number(temp[3]);
    assignedIPs.append(QPair<int,QString> (portNum , newIP));
    return newIP;
}

bool Router::giveBackIP(QString ip)
{
    for (int i = 0; i < assignedIPs.size(); i++)
        if(assignedIPs[i].second == ip){
            assignedIPs.removeAt(i);
            return true;
        }
    return false;
}
