#include "router.hpp"
#include <QHostAddress>

Router::Router(IP _ip,IPversion v ,int _portQueueSize) :Node(_ip ,v , _portQueueSize) {
    assignedIPs.append(QPair<int,QString> (-1 , ip));
}

Router::~Router()
{
    for(int i = 0 ; i < forwardingTable.size() ; i++)
    {
        delete forwardingTable[i]->port;
        delete forwardingTable[i];
    }
}

route::route(){}

route::route(IP& ip, int& _cost, IP nh){
    destination = ip;
    cost = _cost;
    nextHop = nh;

}

IP Router::convertIPv6ToIPv4(IP ipv6Address) {
    QHostAddress ipv6Addr(ipv6Address);
    if (ipv6Addr.protocol() != QAbstractSocket::IPv6Protocol)
        return QString();
    QHostAddress ipv4Addr = QHostAddress(ipv6Addr.toIPv6Address());
    return ipv4Addr.toString();
}

IP Router::convertIPv4ToIPv6(IP ipv4Address)
{
    QHostAddress ipv4Addr(ipv4Address);
    if (ipv4Addr.protocol() != QAbstractSocket::IPv4Protocol)
        return QString();
    QHostAddress ipv6Addr = QHostAddress(ipv4Addr.toIPv6Address());
    return ipv6Addr.toString();
}

void Router::onClock(NetworkState ns)
{
    currentState = ns;
    // qDebug() << "router " + ip + " got clock";
    roundRobinPacketHandler();
}

forward* Router::createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID , int queueSize)
{
    Port *p = new Port(queueSize);
    forward * f = new forward{hopID , subnetMask , subnetID , p};
    forwardingTable.append(f);
    return forwardingTable[forwardingTable.size()-1];
}

void Router::sendToPort(int portNum , QSharedPointer<Packet> pack)
{
    forwardingTable[portNum-1]->port->sendPacket(pack);
}

Port * Router::getPort(int num)
{
    if(num < forwardingTable.size())
        return forwardingTable[num]->port;
    return NULL;
}

Port * Router::getPort(IP ip)
{
    for(int i = 0; i < forwardingTable.size(); i++)
        if(forwardingTable[i]->nextHopIP == ip)
            return forwardingTable[i]->port;
    return NULL;
}

IP Router::requestIP(int portNum)
{
    for(int i = 0 ; i < assignedIPs.size(); i++)
        if(assignedIPs[i].first == portNum)
            return assignedIPs[i].second;
    QVector<int> temp = splitIp(assignedIPs[assignedIPs.size()-1].second);
    if(temp[3] >= 255)
        return NULL; //no capacity
    temp[3]++;
    QString newIP = QString::number(temp[0])+"."+QString::number(temp[1])+"."+QString::number(temp[2])+"."+QString::number(temp[3]);
    assignedIPs.append(QPair<int,QString> (portNum , newIP));
    return newIP;
}

bool Router::giveBackIP(QString ip)
{
    for (int i = 0; i < assignedIPs.size(); i++)
        if(assignedIPs[i].second == ip){
            assignedIPs.removeAt(i);
            return true;
        }
    return false;
}

void Router::roundRobinPacketHandler()
{
    while(true)
    {
        int emptyQueuesCount= 0;
        for (int i = 0; i < forwardingTable.size(); i++)
        {
            if(forwardingTable[i]->port->isQueueEmpty())
            {
                emptyQueuesCount++;
                continue;
            }
            handleDequeuedPacket(forwardingTable[i]->port->dequeue() , i);
        }
        if(emptyQueuesCount == forwardingTable.size())
            break;
    }
}

void Router::handleDequeuedPacket(QSharedPointer<Packet> p , int portNum)
{
    switch(p->getType()){
    case DHCP:
        if(p->getData() == "IP_REQUEST")
        {
            IP achievedIp = requestIP(portNum);
            if(!achievedIp.isNull())
            {
                Packet pack(ip , achievedIp , "DHCP_ANSWER" , DHCP);
                forwardingTable[portNum]->nextHopIP = achievedIp;
                forwardingTable[portNum]->port->sendPacket(QSharedPointer<Packet>::create(pack));
            }
        }
        else if(p->getData() == "IP_GIVEBACK")
        {
            giveBackIP(p->getSourceAddr());
            forwardingTable[portNum]->nextHopIP = "0";
        }
        break;
    case DistanceVec:
        updateDistanceVec(p ,portNum);
        break;
    case Greeting:

        break;
    default:
        break;
    }

}

void Router::updateDistanceVec(QSharedPointer<Packet> p , int portNum){
    QString new_data =  p->getPacket();
    QVector distance_info = spliteString(new_data ,',');
    bool tableChanged = false;
    for(int i=0; i < distance_info.size(); i++){
        QVector<QString> ip_distance = spliteString(distance_info[i] ,':');
        IP curr_ip = ip_distance[0];
        int new_distance = ip_distance[1].toInt();

        auto curr_route = routingTable.find(curr_ip);
        if(curr_route == routingTable.end() || curr_route->cost > new_distance){
            tableChanged = true;
            routingTable[curr_ip] = route(curr_ip , new_distance , p->getDestiantionAddr());
        }
    }
    if(tableChanged)
        sendRouteTebleInfo();
}


QString Router::dataOfRoutingTable(){
    QString data = "";
    for(auto i = routingTable.begin();i != routingTable.end(); i++){
        data += i->destination;
        data += ":";
        data += QString::number(i->cost);
        data += ",";
    }
    return data;
}

void Router::sendRouteTebleInfo(){
    QString routingTableData = dataOfRoutingTable();
    for(auto neighbour : forwardingTable){
        Packet packet = Packet(ip ,neighbour->nextHopIP ,routingTableData ,DistanceVec);
        QSharedPointer<Packet> distanceVecMessage = QSharedPointer<Packet>::create(packet);
        neighbour->port->sendPacket(distanceVecMessage);
    }
}
