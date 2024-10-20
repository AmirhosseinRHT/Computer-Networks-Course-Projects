#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP
#include <iostream>
#include <winsock2.h>

class TCPServer {
public:
    void setPort(int p)
    {
        port = p;
    }

    void Start() {
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize winsock" << std::endl;
            return;
        }

        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            WSACleanup();
            return;
        }
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;

        if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Binding failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }
        if (listen(serverSocket, 1) == SOCKET_ERROR) {
            std::cerr << "Listening failed" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        std::cout << "Waiting for incoming connection..." << std::endl;

        sockaddr_in clientAddress;
        int clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to accept connection" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }
        std::cout << "Client connected" << std::endl;
    }

    std::string recieveMessage()
    {
        char buffer[2048];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0)
            buffer[bytesRead] = '\0';
        return buffer;
    }

    void sendMessage(std::string message)
    {
        send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
    }

private:
    int port;
    WSADATA wsaData;
    SOCKET serverSocket;
    SOCKET clientSocket;
};


#endif // TCPSERVER_HPP
