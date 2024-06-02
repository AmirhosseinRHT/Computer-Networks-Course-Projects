#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>
#include<QVector>

typedef QString IP;

enum ClusterType { RingStar = 1, Mesh = 2};

enum PacketType { Data = 1 , Greeting = 2 , DistanceVec= 3 , DHCP = 4};

enum IPversion {IPV4 = 1 , IPV6 = 2 };

#endif // DEFS_HPP
