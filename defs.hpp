#ifndef DEFS_HPP
#define DEFS_HPP
#include <QString>
#include<QVector>
#include<vector>
#include<string>

typedef QString IP;

enum ClusterType { RingStar = 1, Mesh = 2};

enum PacketType { Data = 1 , Greeting = 2 , DistanceVec= 3 , DHCP = 4};

enum IPversion {IPV4 = 1 , IPV6 = 2 };


QVector<QString> string_split(QString str, char del){
    QVector<QString> resulte_vector;
    QString temp = "";
    for(int i=0; i<(int)str.size(); i++){
        if(str[i] != del){
            temp += str[i];
        }
        else{
            if(temp.size() > 0)
                resulte_vector.push_back(temp);
            temp = "";
        }
    }
    if(temp.size() > 0)
        resulte_vector.push_back(temp);
    return resulte_vector;
}

#endif // DEFS_HPP
