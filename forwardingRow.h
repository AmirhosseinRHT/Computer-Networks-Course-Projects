#ifndef FORWARDINGROW_H
#define FORWARDINGROW_H
#include "node.hpp"

typedef struct{
    IP forwardingIP;
    IP subnetMask;
    IP subnetID;
    int port;
} forwardingRow;

#endif // FORWARDINGROW_H
