#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

class controller  : public QObject
{
    Q_OBJECT
public:
   explicit controller(QObject *parent = nullptr);
};

#endif // CONTROLLER_HPP
