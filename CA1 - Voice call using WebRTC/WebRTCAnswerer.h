#ifndef WEBRTCCLIENT_H
#define WEBRTCCLIENT_H

#include "rtc/rtc.hpp"
#include "TCPServer.h"
#include"AudioOut.h"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>


using namespace std::chrono_literals;
using std::shared_ptr;
using std::weak_ptr;

class WebRTCClientAnswerer {
private:
    shared_ptr<rtc::PeerConnection> pc;
    shared_ptr<rtc::DataChannel> dc;
    int role;
    TCPServer TCPserver;
    std::string recievedConfig;
    std::string desc;
    std::vector <std::string> cand;
    AudioOut *ao=nullptr;
    std::string ip;

public:
    WebRTCClientAnswerer();
    WebRTCClientAnswerer(std::string _ip){
        std::cout << "answerer ----- server\n";
        rtc::InitLogger(rtc::LogLevel::Warning);
        initializePeerConnection();
        TCPserver.setPort(12345);
        TCPserver.Start();
        role = 2;
        ip = _ip;
    }
    void start();
    void close();
    void sendMessage(const char *);
    void setAudioOut(AudioOut * _ao);

private:
    void initializePeerConnection();
    int chooseRole();
    void initializeDataChannel();
    void onDataChannel();
    void handleCommands();
    void printMenu();
    int getCommand();
    void executeCommand(int command, bool& exit);
    void parseDescription();
    void parseCandidate();

    void printConnectionInfo();
    void setPeerConnectionCallbacks();
    void setDataChannelCallbacks();

};

#endif // WEBRTCCLIENT_H
