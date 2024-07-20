#ifndef TABLEROW_H
#define TABLEROW_H

#include "defs.hpp"
#include "port.hpp"

typedef struct route{
    IP destination;
    int cost;
    IP nextHop;
    route();
    route(const IP ip,const int _cost , const IP nh);
} route;

typedef struct{
    IP nextHopIP;
    IP subnetMask;
    IP subnetID;
    Port * port;
    NodeType nextHobType;
} forward;

#endif // TABLEROW_H
