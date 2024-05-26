#include "node.hpp"

node::node(IP _ip , QObject *parent)
    : QObject{parent}
{
    ip = _ip;
}
