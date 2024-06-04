#include "signaller.hpp"
#include <QTime>
#include <QThread>

Signaller::Signaller(int _clockLength , QObject *parent)
    : QObject{parent}
{
    clockLength = _clockLength;
    currentState = InteractionWithDHCP;
}

void Signaller::main()
{

    emit Clock(InteractionWithDHCP);
    QThread::msleep(clockLength);
    emit Clock(NeighborIdentification);
    QThread::msleep(clockLength);
    emit Clock(InteractionWithDHCP);
    QThread::msleep(clockLength);
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock(SendData);
        QThread::msleep(clockLength);
    }
}
