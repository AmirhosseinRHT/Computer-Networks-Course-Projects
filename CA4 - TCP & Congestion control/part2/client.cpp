#include <iostream>
#include <string>
#include<cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include"Client.hpp"
#include"Reno.hpp"


int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> <server_port>" << std::endl;
        return 1;
    }
    std::string my_ip = argv[1];
    std::string serverIP = argv[2];
    int serverPort = std::stoi(argv[3]);

    Client client(serverIP, serverPort);
    Reno reno(100000 , 2000000);
    reno.run(client);

    return 0;
}