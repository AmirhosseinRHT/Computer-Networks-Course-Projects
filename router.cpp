#include "router.hpp"
#include <QHostAddress>

Router::Router(IP _ip,IPversion v ,int _portQueueSize , bool isEdge) :Node(_ip ,v , _portQueueSize) {
    assignedIPs.append(QPair<int,QString> (-1 , ip));
    // routingTable[getBaseIP(_ip)] = route(getBaseIP(_ip) , 0 , getBaseIP(_ip));
    isEdgeRouter = isEdge;
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

route::route(const IP ip,const  int _cost,const IP nh){
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
    if(ns == RouterGreeting)
    {
        sendGreetingPacket();
    }
    else if(ns == NeighborIdentification){
        sendRouteTebleInfo();
    }
    roundRobinPacketHandler();
}

void Router::sendGreetingPacket()
{
    for(auto neighbour : forwardingTable){
        Packet packet = Packet(ip ,neighbour->nextHopIP ,"GREETING" ,Greeting);
        QSharedPointer<Packet> greetingPacket = QSharedPointer<Packet>::create(packet);
        neighbour->port->sendPacket(greetingPacket);
    }
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

void Router::handleDhcpRequest(QSharedPointer<Packet> p, int portNum)
{
    if(p->getData() == "IP_REQUEST")
    {
        IP achievedIp = requestIP(portNum);
        if(!achievedIp.isNull())
        {
            Packet pack(ip , achievedIp , "DHCP_ANSWER" , DHCP);
            routingTable[achievedIp] = route(achievedIp , 1 , achievedIp);
            forwardingTable[portNum]->nextHopIP = achievedIp;
            forwardingTable[portNum]->nextHobType = HOST;
            forwardingTable[portNum]->port->sendPacket(QSharedPointer<Packet>::create(pack));
        }
    }
    else if(p->getData() == "IP_GIVEBACK")
    {
        giveBackIP(p->getSourceAddr());
        // routingTable.erase(p->getSourceAddr());
        forwardingTable[portNum]->nextHopIP = "0";
    }
}

void Router::handleDequeuedPacket(QSharedPointer<Packet> p , int portNum)
{
    switch(p->getType()){
    case DHCP:
        handleDhcpRequest(p, portNum);
        break;
    case DistanceVec:
        updateDistanceVec(p ,portNum);
        break;
    case Data:
        forwardPacket(p ,portNum);
        break;
    case Greeting:
        handleGreetingPacket(p , portNum);
        break;
    default:
        break;
    }
}

void Router::forwardPacket(QSharedPointer<Packet> p, int portNum)
{
    if(getBaseIP(p->getDestiantionAddr()) == getBaseIP(ip))
    {
        for(auto i = routingTable.begin();i != routingTable.end(); i++){
            if(i->destination == p->getDestiantionAddr()){
                for(int j =0 ; j < forwardingTable.size();j++)
                    if(forwardingTable[j]->nextHopIP == i->nextHop){
                        p->addLog("forwarded by router:" + ip +" to hop:" + i->nextHop);
                        p->icreaseInQueueCycle();
                        forwardingTable[j]->port->sendPacket(p);
                        return;
                    }
            }

        }
    }
    else
    {
        for(auto i = routingTable.begin();i != routingTable.end(); i++){
            if(i->destination == getBaseIP(p->getDestiantionAddr())){
                for(int j =0 ; j < forwardingTable.size();j++)
                    if(forwardingTable[j]->nextHopIP == i->nextHop){
                        p->addLog("forwarded by router:" + ip +" to hop:" + i->nextHop);
                        p->icreaseInQueueCycle();
                        forwardingTable[j]->port->sendPacket(p);
                        return;
                    }
            }

        }
    }
}

void Router::handleGreetingPacket(QSharedPointer<Packet> p , int portNum)
{
    if(forwardingTable[portNum]->nextHopIP == "NOTHING" || forwardingTable[portNum]->nextHopIP == "0")
    forwardingTable[portNum]->nextHopIP = p->getSourceAddr();
    if(getBaseIP(p->getSourceAddr()) == getBaseIP(ip))
        forwardingTable[portNum]->nextHobType = LOCAL_ROUTER;
    else
        forwardingTable[portNum]->nextHobType = EXTERNAL_ROUTER;
}

void Router::updateDistanceVec(QSharedPointer<Packet> p , int portNum){
    // qDebug() << "router " << ip << " receive distanceVec Packet from " << p->getSourceAddr()<< "\n";
    QString new_data =  p->getData();
    QVector <QString> distance_info = spliteString(new_data ,',');
    routingTableChanged = false;

    for(int i=0; i < distance_info.size(); i++){
        QVector<QString> ip_distance = spliteString(distance_info[i] ,':');
        IP curr_ip = ip_distance[0];
        int new_distance = ip_distance[1].toInt();

        auto curr_route = routingTable.find(curr_ip);
        if(curr_route == routingTable.end() || curr_route->cost > (new_distance +1)){
            routingTableChanged = true;
            routingTable[curr_ip] = route(curr_ip , new_distance+1 , p->getSourceAddr());
        }
    }
    if(routingTableChanged)
        sendRouteTebleInfo();
}


QString Router::dataOfRoutingTable(NodeType nextHopType){
    QString data = "";
    for(auto i = routingTable.begin();i != routingTable.end(); i++){
        if((nextHopType == EXTERNAL_ROUTER) && getBaseIP(i->destination)==getBaseIP(ip))
            continue;
        data += i->destination;
        data += ":";
        data += QString::number(i->cost);
        data += ",";
    }
    data+=getBaseIP(ip)+":0,";
    return data;
}

void Router::sendRouteTebleInfo(){
    for(auto neighbour : forwardingTable){
        if(neighbour->nextHobType != HOST)
        {
            Packet packet = Packet(ip ,neighbour->nextHopIP ,dataOfRoutingTable(neighbour->nextHobType) ,DistanceVec);
            QSharedPointer<Packet> distanceVecMessage = QSharedPointer<Packet>::create(packet);
            neighbour->port->sendPacket(distanceVecMessage);
        }
    }
}

void Router::updatePacketLogs(QSharedPointer<Packet> p , QString log)
{
    p->icreaseInQueueCycle();
    p->addLog(log);
}

void Router::printRoutingTable()
{
    for(auto i = routingTable.begin();i != routingTable.end(); i++)
        qDebug() << "Destination : " + i->destination + " cost : " + QString::number(i->cost) + " next hop: " + i->nextHop ;

}

void Router::printTable(QString _ip)
{
    if(ip == _ip)
    {
        printRoutingTable();
    }
}
