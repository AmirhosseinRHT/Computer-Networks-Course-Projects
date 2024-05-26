#ifndef PACKET_HPP
#define PACKET_HPP
#include <vector>
#include <QObject>
#include <QTime>
#include "defs.hpp"
#include <iostream>

class packet : public QObject
{
public:
    explicit packet(IP source ,IP dest ,IP _data , QObject *parent = nullptr);
    std::string getPacket();
    IP getDestiantionAddr() {return destinationAddr;}
    IP getSourceAddr() {return sourceAddr;}
    std::string getTime() {return generateTime;}
    void printLog();
private:
    IP sourceAddr;
    IP destinationAddr;
    IP data;
    std::string generateTime;
    std::vector<std::string> log;
};

#endif // PACKET_HPP
