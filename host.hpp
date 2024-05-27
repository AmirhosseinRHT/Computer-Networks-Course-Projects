#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include "packet.hpp"
#include "node.hpp"
#include "defs.hpp"


class Host  : public node
{
    Q_OBJECT
public:
    explicit Host(IP ip);
    packet * packetGenerator(IP destiantionIp);

};

#endif // HOST_HPP
