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
private:
    Port * port;
};

#endif // HOST_HPP
