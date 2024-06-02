#ifndef SIGNALLER_HPP
#define SIGNALLER_HPP

#include <QObject>

class Signaller : public QObject
{
    Q_OBJECT
public:
    explicit Signaller(int _clocklength , QObject *parent = nullptr);
    void main();

private:
    int clockLength;
signals:
    void Clock();
};

#endif // SIGNALLER_HPP
