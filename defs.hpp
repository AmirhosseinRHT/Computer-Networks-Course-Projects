#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>
#include<QVector>
#include <QRegularExpression>

typedef QString IP;

enum ClusterType { RingStar, Mesh , Torus};

enum PacketType { Data, Greeting , DistanceVec , DHCP };

enum IPversion {IPV4, IPV6 , INVALID};

enum NodeType {HOST , LOCAL_ROUTER ,EXTERNAL_ROUTER};

enum NetworkState {InteractionWithDHCP , RouterGreeting , NeighborIdentification , SendData};

QVector<QString> spliteString(const QString &str, char del);

QString getBaseIP(const QString& _ip);

IPversion getIPAddressType(IP address);

IP convertIPv4ToIPv6(IP ipv4Address);

IP convertIPv6ToIPv4(IP ipv6Address);

IP getCompatibleIP(IP current , IPversion ver);



#endif // DEFS_HPP
