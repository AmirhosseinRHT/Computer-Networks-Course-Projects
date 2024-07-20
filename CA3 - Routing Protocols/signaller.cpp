#include "signaller.hpp"
#include <QTime>
#include <QThread>

Signaller::Signaller(int _clockLength , QObject *parent): QObject{parent}
{
    clockLength = _clockLength;
    currentState = InteractionWithDHCP;
}

void Signaller::main()
{

    emit Clock(InteractionWithDHCP);
    QThread::msleep(clockLength);
    emit Clock(RouterGreeting);
    QThread::msleep(clockLength);
    for(int i =0 ; i < 20 ; i++)
    {
        emit Clock(NeighborIdentification);
        QThread::msleep(20);
    }
    qDebug() << "\nREADY TO SEND MESSAGE!";
    while(true)
    {
        emit Clock(SendData);
        QThread::msleep(clockLength);
    }
}
