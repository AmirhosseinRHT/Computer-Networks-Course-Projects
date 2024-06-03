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
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock(currentState);
        QThread::msleep(clockLength);
    }
}
