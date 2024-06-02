#ifndef SIGNALLER_HPP
#define SIGNALLER_HPP

#include <QObject>

class Signaller : public QObject
{
    Q_OBJECT
public:
    explicit Signaller(QObject *parent = nullptr);
    void main();

signals:
    void Clock();
};

#endif // SIGNALLER_HPP
