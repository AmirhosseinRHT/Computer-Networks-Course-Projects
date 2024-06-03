#include "defs.hpp"

QVector<QString> spliteString(QString &str, char del)
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
