#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP

#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
public:
   explicit Controller(QObject *parent = nullptr);
};

#endif // CONTROLLER_HPP
