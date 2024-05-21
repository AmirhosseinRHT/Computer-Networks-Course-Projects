#ifndef ROUTER_HPP
#define ROUTER_HPP

#include <QObject>
#include <string>
#include "wire.hpp"
#include "forwardingRow.h"

class router : public QObject
{
    Q_OBJECT
public:
    explicit router(std::string ip , QObject *parent = nullptr);
    void addPort(wire* port) { ports.push_back(port); }
private:
    std::string IP;
    std::vector<forwardingRow> forwardingTable;
    std::vector<wire*> ports;
};

#endif // ROUTER_HPP
