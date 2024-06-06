#include "router.hpp"

Router::Router(IP _ip,IPversion v ,int _portQueueSize , bool isEdge) :Node(_ip ,v , _portQueueSize) {
    assignedIPs.append(QPair<int,QString> (-1 , getCompatibleIP(ip , IPV4)));
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


void Router::onClock(NetworkState ns)
{
    currentState = ns;
    if(ns == RouterGreeting)
    {
        sendGreetingPacket();
    }
    else if(ns == NeighborIdentification){
        if(routeAlgo == OSBF)
            sendMyNeighbour();
        else
                sendRouteTebleInfo();


    }
    roundRobinPacketHandler();
}

QString Router::dataOfNeighbour(){
    QString data = "";
    data += ip;
    data += "$";
    for(auto neighbour : forwardingTable){
        if(neighbour->nextHobType == EXTERNAL_ROUTER)
            data += getBaseIP(neighbour->nextHopIP);
        else
            data += neighbour->nextHopIP;
        data += ",";
    }
    return data;
}

void Router::sendMyNeighbour(){
    QString data = dataOfNeighbour();
    for(auto neighbour : forwardingTable){
        Packet packet = Packet(ip ,neighbour->nextHopIP ,data , DistanceVec, ver);
        QSharedPointer<Packet> greetingPacket = QSharedPointer<Packet>::create(packet);
        neighbour->port->sendPacket(greetingPacket);
    }
}

void Router::sendGreetingPacket()
{
    for(auto neighbour : forwardingTable){
        Packet packet = Packet(ip ,neighbour->nextHopIP ,"GREETING" ,Greeting , ver);
        QSharedPointer<Packet> greetingPacket = QSharedPointer<Packet>::create(packet);
        neighbour->port->sendPacket(greetingPacket);
    }
}

forward* Router::createForwardingRow(IP hopID ,IP subnetMask ,IP subnetID , int queueSize , NodeType type)
{
    Port *p = new Port(queueSize);
    forward * f = new forward{hopID , subnetMask , subnetID , p , type};
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
            Packet pack(ip , getCompatibleIP(achievedIp , ver) , "DHCP_ANSWER" , DHCP , ver);
            routingTable[achievedIp] = route(getCompatibleIP(achievedIp , ver) , 1 , getCompatibleIP(achievedIp , ver));
            forwardingTable[portNum]->nextHopIP = getCompatibleIP(achievedIp , ver);
            forwardingTable[portNum]->nextHobType = HOST;
            forwardingTable[portNum]->port->sendPacket(QSharedPointer<Packet>::create(pack));
        }
    }
    else if(p->getData() == "IP_GIVEBACK")
    {
        giveBackIP(getCompatibleIP(p->getSourceAddr() , IPV4));
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
        if(routeAlgo == OSBF)
            updateTopology(p , portNum);
        else
            updateDistanceVec(p);
        break;
    case Data:
        forwardPacket(p);
        break;
    case Greeting:
        handleGreetingPacket(p , portNum);
        break;
    default:
        break;
    }
}


QString Router::Dijkstra(IP s, IP d){
    QVector<IP> qu;
    qu.append(s);
    int siz =0;
    QMap<IP , IP> f;
    f[s] = "-1";
    while (siz < qu.size()) {

        auto curr = qu[siz];
        if(curr == d)
            break;
        for(IP neighbor : networkTopology[curr]){
            if(f.find(neighbor) == f.end()){
                f[neighbor] = curr;
                qu.push_back(neighbor);

            }
        }
        siz++;
    }
    auto curr = d;
    while (f[curr] != s) {
        curr = f[curr];
    }
    return curr;
}

void Router::forwardPacket(QSharedPointer<Packet> p)
{
    IP destIP;
    QSharedPointer<Packet> forwardingPacket;
    if(p->getIPVersion() != ver)
    {
        if(p->maskedPacket)
        {
            forwardingPacket = p->getInnerPacket();
            destIP =p->getInnerPacket()->getDestiantionAddr();
        }
        else
        {
            Packet pack(ip , getCompatibleIP(p->getDestiantionAddr() , ver) , "MASKER" , Data , ver);
            pack.setInnerPacket(p);
            destIP = getCompatibleIP(p->getDestiantionAddr() , ver);
            forwardingPacket = QSharedPointer<Packet>::create(pack);
        }
    }
    else
    {
        destIP = p->getDestiantionAddr();
        forwardingPacket = p;
    }
    if(getBaseIP(getCompatibleIP(forwardingPacket->getDestiantionAddr() , IPV4)) != getBaseIP(getCompatibleIP(ip , IPV4)))
        destIP = getBaseIP(getCompatibleIP(forwardingPacket->getDestiantionAddr() , IPV4));

    if(routeAlgo == OSBF){
        if(getBaseIP(ip) != getBaseIP(destIP))
            destIP = getBaseIP(destIP);
        auto nextHobIp = Dijkstra(ip ,destIP);
        for(int j =0 ; j < forwardingTable.size();j++)
            if(forwardingTable[j]->nextHopIP == nextHobIp){
                forwardingPacket->addLog("forwarded by router:" + ip +" to hop: " + nextHobIp);
                forwardingPacket->icreaseInQueueCycle();
                forwardingTable[j]->port->sendPacket(forwardingPacket);
                return;
            }
        return;
    }
    for(auto i = routingTable.begin();i != routingTable.end(); i++){
        if(i->destination == destIP){
            for(int j =0 ; j < forwardingTable.size();j++)
                if(forwardingTable[j]->nextHopIP == i->nextHop){
                    forwardingPacket->addLog("forwarded by router:" + ip +" to hop: " + i->nextHop);
                    forwardingPacket->icreaseInQueueCycle();
                    forwardingTable[j]->port->sendPacket(forwardingPacket);
                    return;
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

void Router::updateDistanceVec(QSharedPointer<Packet> p){
    // qDebug() << "router " << ip << " receive distanceVec Packet from " << p->getSourceAddr()<< "\n";
    QString new_data =  p->getData();
    QVector <QString> distance_info = spliteString(new_data ,',');
    routingTableChanged = false;

    for(int i=0; i < distance_info.size(); i++){
        QVector<QString> ip_distance = spliteString(distance_info[i] ,'-');
        IP curr_ip = ip_distance[0];
        int new_distance = ip_distance[1].toInt();
        curr_ip= getCompatibleIP(curr_ip , ver);
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
        data += "-";
        data += QString::number(i->cost);
        data += ",";
    }
    data+=getBaseIP(getCompatibleIP(ip , IPV4))+"-0,";
    return data;
}

void Router::sendRouteTebleInfo(){
    for(auto neighbour : forwardingTable){
        if(neighbour->nextHobType != HOST)
        {
            Packet packet = Packet(ip ,neighbour->nextHopIP ,dataOfRoutingTable(neighbour->nextHobType) ,DistanceVec , ver);
            QSharedPointer<Packet> distanceVecMessage = QSharedPointer<Packet>::create(packet);
            neighbour->port->sendPacket(distanceVecMessage);
        }
    }
}

void Router::sendPacketToAllports(QSharedPointer<Packet> p ,int pn){
    for(auto neighbour : forwardingTable){
        neighbour->port->sendPacket(p);
    }
}

void Router::updateTopology(QSharedPointer<Packet> p , int pn){
    auto dataParts = spliteString(p->getData(), '$');
    auto ip_list = spliteString(dataParts[1] , ',');
    auto centerIp = dataParts[0];

    centerIp = convertIPv6ToIPv4(centerIp);
    if(getBaseIP(ip) != getBaseIP(centerIp))
        return;
    if(networkTopology.find(centerIp) == networkTopology.end()){
        networkTopology[centerIp] = ip_list;
        sendPacketToAllports(p , pn);
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
    if(ip == getCompatibleIP(_ip , ver))
    {
        printRoutingTable();
    }
}
