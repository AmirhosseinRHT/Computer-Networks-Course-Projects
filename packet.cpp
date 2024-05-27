#include "packet.hpp"

packet::packet(IP source ,IP dest ,IP _data , PacketType _type)
{
    sourceAddr = source;
    destinationAddr = dest;
    data = _data;
    generateTime = QTime::currentTime().toString("hh:mm:ss");
    type = _type;
}


QString packet::getPacket(){
    return data + "_" + destinationAddr + "_" + sourceAddr + "_" + generateTime;
}

void packet::printLog()
{
    for(int i = 0 ; i < log.size(); i++)
        qDebug() << i << " : " << log[i] << "\n";
}

