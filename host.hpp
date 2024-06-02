#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include "port.hpp"
#include "node.hpp"
#include "defs.hpp"


class Host : public Node
{
    Q_OBJECT
public:
    explicit Host(IP ip ,IPversion v  , int _portQueueSize);
    ~Host();
    Port * getPort() {return port;}
    void getIpFromDHCPServer();
private:
    Port * port;
public slots:
    void onClock();
};

#endif // HOST_HPP
