#include "router.hpp"

router::router(std::string ip , QObject* parent)
: QObject{parent}
{
    IP = ip;
}
