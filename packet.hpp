#ifndef PACKET_HPP
#define PACKET_HPP
#include <QObject>
#include <QTime>
#include "defs.hpp"

class Packet
{
public:
    explicit Packet(IP source ,IP dest ,QString _data ,PacketType _type);
    virtual ~Packet(){}
    QString getPacket();
    IP getDestiantionAddr() {return destinationAddr;}
    IP getSourceAddr() {return sourceAddr;}
    IP getData() {return data;}
    QString getTime() {return generateTime;}
    PacketType getType() {return type;}
    void setDestination(IP dest) {destinationAddr = dest;}
    void setSource(IP source) {sourceAddr = source;}
    void setData(QString d){ data = d;}
    void setTime(QString time){generateTime = time;}
    void setPacketType(PacketType t){ type = t;}
    void icreaseInQueueCycle(int val = 1);
    bool isMaskedAPacket(){return maskedPacket;}
    QSharedPointer<Packet> getInnerPacket(){return innerPacket;}
    void addLog(QString log);
    void printLogs();
private:
    IP sourceAddr;
    IP destinationAddr;
    QString data;
    PacketType type;
    QString generateTime;
    QVector<QPair<int ,QString>> logs;
    int inQueueCycle;
    bool maskedPacket;
    QSharedPointer<Packet> innerPacket;
};

#endif // PACKET_HPP
