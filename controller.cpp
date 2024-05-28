#include "controller.hpp"
#include <QTime>

Controller::Controller(QObject * parent): QObject{parent}
{}



void Controller::main()
{
    while(true)
    {
        emit Pulse();

        // QObject().thread()->usleep();
        // _sleep(100);
    }
}
