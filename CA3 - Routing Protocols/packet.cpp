#include "packet.hpp"

Packet::Packet(IP source ,IP dest ,IP _data , PacketType _type , IPversion _v)
{
    sourceAddr = source;
    destinationAddr = dest;
    data = _data;
    generateTime = QTime::currentTime().toString("hh:mm:ss");
    type = _type;
    inQueueCycle = 0;
    maskedPacket = false;
    innerPacket = NULL;
    ipVer = _v;
}
void Packet::setInnerPacket(QSharedPointer<Packet> inP)
{
    innerPacket = inP ;
    maskedPacket = true;
}

void Packet::icreaseInQueueCycle(int val)
{
    inQueueCycle+=val;
    if(maskedPacket)
        innerPacket->icreaseInQueueCycle(val);
}

QString Packet::getPacket(){
    return data + "_" + destinationAddr + "_" + sourceAddr + "_" + generateTime;
}

void Packet::printLogs()
{
    for(int i = 0 ; i < logs.size(); i++)
        qDebug() <<"Cycle " + QString::number(logs[i].first) << " : " << logs[i].second << "\n";
}

void  Packet::addLog(QString log)
{
    logs.append(QPair<int,QString> (inQueueCycle , log));
    if(maskedPacket)
        innerPacket->addLog(log);
}
