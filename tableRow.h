#ifndef TABLEROW_H
#define TABLEROW_H

#include "defs.hpp"
#include "node.hpp"

typedef struct{
    IP destination;
    int cost;
    IP nextHop;
} route;

typedef struct{
    IP forwardingIP;
    IP subnetMask;
    IP subnetID;
    int portNumber;
    node * port;
} forward;

#endif // TABLEROW_H
