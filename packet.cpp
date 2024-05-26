#include "packet.hpp"

packet::packet(IP source ,IP dest ,IP _data ,QObject *parent):QObject{parent}
{
    sourceAddr = source;
    destinationAddr = dest;
    data = _data;
    generateTime = QTime::currentTime().toString("hh:mm:ss").toStdString();
}

std::string packet::getPacket(){
    return data + "_" + destinationAddr + "_" + sourceAddr + "_" + generateTime;
}

void packet::printLog()
{
    for(int i = 0 ; i < log.size(); i++)
        std::cout << i << " : " << log[i] << std::endl;
}

