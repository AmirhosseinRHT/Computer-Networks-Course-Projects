#ifndef HOST_HPP
#define HOST_HPP

#include <QObject>
#include "port.hpp"
#include "node.hpp"
#include "defs.hpp"


class Host : public Node
{
    Q_OBJECT
public:
    explicit Host(IP ip , int portQueueSize);
    ~Host();
private:
    Port * outport;
};

#endif // HOST_HPP
