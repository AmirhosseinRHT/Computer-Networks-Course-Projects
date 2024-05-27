#ifndef TABLEROW_H
#define TABLEROW_H

#include "defs.hpp"
#include "port.hpp"

typedef struct{
    IP destination;
    int cost;
    IP nextHop;
} route;

typedef struct{
    IP nextHopIP;
    IP subnetMask;
    IP subnetID;
    Port * port;
} forward;

#endif // TABLEROW_H
