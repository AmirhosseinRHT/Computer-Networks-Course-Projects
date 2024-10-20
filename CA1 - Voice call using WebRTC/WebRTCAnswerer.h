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
    WebRTCClientAnswerer(std::string _ip);
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
    void parseCandidate();
    void setPeerConnectionCallbacks();
    void setDataChannelCallbacks();
};

#endif // WEBRTCCLIENT_H
