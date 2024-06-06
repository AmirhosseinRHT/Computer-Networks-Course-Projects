#include "defs.hpp"

QVector<QString> spliteString(const QString &str, char del)
{
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

QString getBaseIP(const QString& _ip) {
    QStringList octets = _ip.split('.');
    if (octets.size() >= 2) {
        return octets[0] + "." + octets[1];
    }
    return "";
}
