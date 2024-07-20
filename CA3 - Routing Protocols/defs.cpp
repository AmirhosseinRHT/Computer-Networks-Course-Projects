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


IPversion getIPAddressType(IP address) {
    QRegularExpression ipv4RegEx("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");
    QRegularExpression ipv6RegEx("^(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))$");
    if (ipv4RegEx.match(address).hasMatch())
        return IPversion::IPV4;
    else if (ipv6RegEx.match(address).hasMatch())
        return IPversion::IPV6;
    else
        return IPversion::INVALID;
}


IP convertIPv4ToIPv6(IP ipv4Address) {
    QStringList octets = ipv4Address.split(".");
    if (octets.size() != 4) {
        return QString(); // Invalid IPv4 address
    }

    QString ipv6Address = "::FFFF:";
    for (int i = 0; i < 4; ++i) {
        bool ok;
        int octet = octets[i].toInt(&ok, 10);
        if (!ok || octet < 0 || octet > 255) {
            return QString(); // Invalid octet value
        }

        ipv6Address.append(QString("%1").arg(octet, 2, 16, QLatin1Char('0')));
        if (i < 3) {
            ipv6Address.append(":");
        }
    }

    return ipv6Address;
}

IP convertIPv6ToIPv4(IP ipv6Address) {
    QStringList parts = ipv6Address.split(":");
    if(ipv6Address.split(".").size() == 4){
        return ipv6Address;
    }
    if (parts.size() != 7 || !parts[0].isEmpty() || !parts[1].isEmpty() || parts[2] != "FFFF") {
        return QString(); // Invalid IPv4-mapped IPv6 address
    }

    QStringList octets;
    for (int i = 3; i < 7; ++i) {
        bool ok;
        int value = parts[i].toInt(&ok, 16);
        if (!ok || value < 0 || value > 255) {
            return QString(); // Invalid octet value
        }
        octets.append(QString::number(value));
    }

    return octets.join(".");
}


IP getCompatibleIP(IP current , IPversion ver)
{
    if(getBaseIP(current) == current)
        return current;
    if(getIPAddressType(current) == IPV6 && ver == IPV4)
        return convertIPv6ToIPv4(current);
    else if(getIPAddressType(current) == IPV4 && ver == IPV6)
        return convertIPv4ToIPv6(current);
    else
        return current;
}
