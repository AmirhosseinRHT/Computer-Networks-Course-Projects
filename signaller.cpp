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
    // qDebug() << "111111111111111111111111111111111111111111111111111111";
    QThread::msleep(clockLength);
    for(int i =0 ; i < 20 ; i ++)
    {
        emit Clock(NeighborIdentification);
        QThread::msleep(100);
    }
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock(SendData);
        QThread::msleep(clockLength);
    }
}
