#include "commandreader.hpp"
#include <QThread>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
CommandReader::CommandReader(QObject *parent) : QObject(parent){}

void CommandReader::readCommands()
{
    while (true)
    {
        std::string command;
        std::cout << "COMMAND: ";
        std::getline(std::cin, command);
        istringstream istrstream(command);
        string str;
        vector<std::string> commandSplits;
        while (getline(istrstream,str,' '))
            commandSplits.push_back(str);
        if(commandSplits[0].compare("p") == 0)
            emit printRoutingTable(QString::fromStdString(commandSplits[1]));
        else if(commandSplits[0].compare("s") == 0)
            emit requestSendPacket(QString::fromStdString(commandSplits[1]) , QString::fromStdString(commandSplits[2]));
        QThread::msleep(100);
    }
}
