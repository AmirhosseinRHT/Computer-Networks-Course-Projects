#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include <string>
#include "packet.hpp"
#include "wire.hpp"
class Host  : public QObject
{
    Q_OBJECT
public:
    explicit Host(std::string ip , QObject *parent = nullptr);
    packet * packetGenerator(std::string destiantionIp);
private:
    std::string IP;
    wire *outputPort;
};

#endif // HOST_HPP
