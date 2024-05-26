#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include "node.hpp"
// #include "wire.hpp"
#include "forwardingRow.h"


class router : public node
{
    Q_OBJECT
public:
    explicit router(IP _ip);
    void addPort(node* port);
private:
    std::vector<forwardingRow> forwardingTable;
    std::vector<node*> ports;
};

#endif // ROUTER_HPP
