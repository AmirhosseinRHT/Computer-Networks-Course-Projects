#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
#include "tableRow.h"
#include "packet.hpp"

class router : public node
{
public:
    explicit router(IP _ip );
    void addPort(node* port);
private:
    QVector<route> routingTable;
    QVector<forward> forwardingTable;
};

#endif // ROUTER_HPP
