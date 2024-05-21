#ifndef WIRE_HPP
#define WIRE_HPP

#include <QObject>
#include <QQueue>
#include <string>
#include "packet.hpp"

class wire : public QObject
{
    Q_OBJECT
public:
    explicit wire(std::string head, std::string tail , QObject *parent = nullptr);
private:
    std::string head;
    std::string tail;
    QQueue<packet *> toHead;
    QQueue<packet *> totail;
};

#endif // WIRE_HPP
