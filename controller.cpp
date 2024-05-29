#include "controller.hpp"
#include <QTime>

Controller::Controller(QObject * parent): QObject{parent}
{}


void Controller::main()
{
    while(true)
    {
        emit Pulse();
        QThread::msleep(1000);
    }
}
