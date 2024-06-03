#ifndef TABLEROW_H
#define TABLEROW_H

#include "defs.hpp"
#include "port.hpp"

typedef struct route{
    IP destination;
    int cost;
    IP nextHop;
    route();
    route(IP& ip,int& _cost , IP nh);
} route;

typedef struct{
    IP nextHopIP;
    IP subnetMask;
    IP subnetID;
    Port * port;
} forward;

#endif // TABLEROW_H
