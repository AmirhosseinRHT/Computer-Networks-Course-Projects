#ifndef COMMANDREADER_H
#define COMMANDREADER_H

#include <QObject>

class CommandReader : public QObject
{
    Q_OBJECT
public:
    explicit CommandReader(QObject *parent = nullptr);
    void readCommands();
signals:
    void printRoutingTable(QString Command);
    void requestSendPacket(QString src , QString dest);
};

#endif // COMMANDREADER_H
