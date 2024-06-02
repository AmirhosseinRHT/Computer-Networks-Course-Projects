#include "signaller.hpp"
#include <QTime>
#include <QThread>

Signaller::Signaller(QObject *parent)
    : QObject{parent}
{}

void Signaller::main()
{
    while(true)
    {
        qDebug() << "emitter emitted";
        emit Clock();
        QThread::msleep(5000);
    }
}
