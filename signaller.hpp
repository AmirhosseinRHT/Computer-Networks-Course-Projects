#ifndef SIGNALLER_HPP
#define SIGNALLER_HPP

#include <QObject>
#include "defs.hpp"

class Signaller : public QObject
{
    Q_OBJECT
public:
    explicit Signaller(int _clocklength , QObject *parent = nullptr);
    void main();

private:
    NetworkState currentState;
    int clockLength;
signals:
    void Clock(NetworkState ns);
};

#endif // SIGNALLER_HPP
