#include "host.hpp"

Host::Host(IP _ip ,IPversion v ,int _portQueueSize): Node(_ip , v , _portQueueSize)
{
    port = new Port(_portQueueSize);
}

Host::~Host()
{
    delete port;
}
