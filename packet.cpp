#include "packet.hpp"

packet::packet(std::string source , std::string dest ,
               std::string _data , QObject *parent):QObject{parent}
{
    sourceAddr = source;
    destinationAddr = dest;
    data = _data;
    generateTime = QTime::currentTime().toString("hh:mm:ss").toStdString();
}

std::string packet::getPacket(){
    return data + "_" + destinationAddr + "_" + sourceAddr + "_" + generateTime;
}

