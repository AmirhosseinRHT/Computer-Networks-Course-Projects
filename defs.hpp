#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>

typedef QString IP;

enum ClusterType { RingStar = 1, Mesh = 2};

enum PacketType { Data = 1 , DistanceVec= 2};

#endif // DEFS_HPP
