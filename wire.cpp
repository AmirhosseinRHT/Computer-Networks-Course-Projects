#include "wire.hpp"

wire::wire(std::string _head , std::string _tail , QObject * parent)
: QObject{parent}
{
    head = _head;
    tail = _tail;
}
