#include "packet.hpp"

Packet::Packet(IP source ,IP dest ,IP _data , PacketType _type)
{
    sourceAddr = source;
    destinationAddr = dest;
    data = _data;
    generateTime = QTime::currentTime().toString("hh:mm:ss");
    type = _type;
    inQueueCycle = 0;
}


QString Packet::getPacket(){
    return data + "_" + destinationAddr + "_" + sourceAddr + "_" + generateTime;
}

void Packet::printLog()
{
    for(int i = 0 ; i < log.size(); i++)
        qDebug() << i << " : " << log[i] << "\n";
}

