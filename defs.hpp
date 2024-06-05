#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>
#include<QVector>

typedef QString IP;

enum ClusterType { RingStar, Mesh , Torus};

enum PacketType { Data ,Greeting, DistanceVec , DHCP };

enum IPversion {IPV4, IPV6};


enum NodeType {HOST , LOCAL_ROUTER ,EXTERNAL_ROUTER};

enum NetworkState {InteractionWithDHCP , NeighborIdentification , SendData};


QVector<QString> spliteString(const QString &str, char del);

#endif // DEFS_HPP
