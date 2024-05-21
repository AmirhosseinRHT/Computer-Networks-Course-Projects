#include "host.hpp"

Host::Host(std::string ip , QObject * parent)
: QObject{parent}
{
    IP = ip;
}
