#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>
#include<QVector>

typedef QString IP;

enum ClusterType { RingStar = 1, Mesh = 2 , Torus};

enum PacketType { Data = 1 , Greeting = 2 , DistanceVec= 3 , DHCP = 4};

enum IPversion {IPV4 = 1 , IPV6 = 2 };


enum NodeType {HOST , LOCAL_ROUTER , EXTERNAL_ROUTER};

enum NetworkState {InteractionWithDHCP , NeighborIdentification , SendData};


QVector<QString> spliteString(const QString &str, char del);

#endif // DEFS_HPP
