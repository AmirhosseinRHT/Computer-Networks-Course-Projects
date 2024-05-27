#include "host.hpp"

Host::Host(IP _ip , int portQueueSize): Node(_ip)
{
    outport = new Port(portQueueSize);
}

Host::~Host()
{
    delete outport;
}
