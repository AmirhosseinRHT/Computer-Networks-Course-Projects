#ifndef _RENO_HPP
#define _RENO_HPP

#include"Client.hpp"
#include<thread>
#include<chrono>

class Reno
{
private:
    int threshold;
    int first_speed;
public:
    Reno();
    Reno(int ther , int _first_speed){threshold = ther;first_speed=_first_speed;}

    void run(Client& client);
    ~Reno();
};



#endif