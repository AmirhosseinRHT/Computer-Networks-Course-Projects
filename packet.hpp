#ifndef PACKET_HPP
#define PACKET_HPP
#include <string>
#include <vector>
#include <QObject>
#include <QTime>

class packet : public QObject
{
public:
    explicit packet(std::string source , std::string dest , std::string _data , QObject *parent = nullptr);
    std::string getPacket();
private:
    std::string sourceAddr;
    std::string destinationAddr;
    std::string data;
    std::string generateTime;
    std::vector<std::string> log;

};

#endif // PACKET_HPP
