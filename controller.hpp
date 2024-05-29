#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>
#include "cluster.hpp"

class Controller : public QObject
{
    Q_OBJECT
public:
   explicit Controller(QObject *parent = nullptr);
    void main();


signals:
   void Pulse();
};

#endif // CONTROLLER_HPP
