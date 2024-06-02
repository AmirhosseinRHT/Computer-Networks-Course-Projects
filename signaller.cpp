#include "signaller.hpp"
#include <QTime>
#include <QThread>

Signaller::Signaller(int _clockLength , QObject *parent)
    : QObject{parent}
{
    clockLength = _clockLength;
}

void Signaller::main()
{
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock();
        QThread::msleep(clockLength);
    }
}
