#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int portQueueSize): Node(_ip , v)
{
    outport = new Port(portQueueSize);
}

Host::~Host()
{
    delete outport;
}
