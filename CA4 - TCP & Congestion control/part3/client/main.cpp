#include "Reciever.hpp"
#include "Sender.hpp"
#include <iostream>

int main(int argc , char * argv[]) {
    if(argc != 2)
    {
        std::cout << "Wrong arguments !!" << std::endl;
        return 0;
    }
    if((int(argv[1][0]) - 48 ) == 1)
    {   
        Sender sender("127.0.0.1", "192.168.1.1", 8080);
        sender.start();
    }
    else if((int(argv[1][0]) - 48 ) == 2)
    {
        Reciever reciever("127.0.0.1", "192.168.1.3", 8080);
        reciever.start();
    }
    return 0;
}