#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP
#include <iostream>
#include <winsock2.h>
#include <string>


class TCPClient {
public:
    TCPClient(const std::string& serverIP, int port) : serverIP(serverIP), port(port) {}

    void Connect() {
        // Initialize Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "Failed to initialize winsock" << std::endl;
            return;
        }

        // Create socket
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Failed to create socket" << std::endl;
            WSACleanup();
            return;
        }

        // Connect to the server
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());

        if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
            std::cerr << "Failed to connect to server" << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
        std::cout << "Connected to server" << std::endl;
    }

    void SendMessage(std::string message)
    {
        std::cout << "sent message to server: " << message << std::endl;
        send(clientSocket, message.c_str(), strlen(message.c_str()), 0);
    }

    std::string recieveMessage()
    {
        char buffer[1024];
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0';
            std::cout << "Received message from server: " << buffer << std::endl;
            return buffer;
        }
    }

private:
    std::string serverIP;
    int port;
    WSADATA wsaData;
    SOCKET clientSocket;
};
#endif // TCPCLIENT_HPP
