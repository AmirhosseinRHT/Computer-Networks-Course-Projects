#ifndef WEBRTCOFERRER_H
#define WEBRTCOFERRER_H

#include "rtc/rtc.hpp"
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include "TCPClient.h"
#include"AudioOut.h"

using namespace std::chrono_literals;
using std::shared_ptr;
using std::weak_ptr;


class WebRTCClientOferrer {
private:
    shared_ptr<rtc::PeerConnection> pc;
    shared_ptr<rtc::DataChannel> dc;
    int role;
    std::string desc;
    std::string ip;
    std::vector <std::string> cand;
    AudioOut *ao = nullptr;

public:
    WebRTCClientOferrer();
    WebRTCClientOferrer(std::string _ip);
    void start();
    void close();
    void sendMessage(const char *);
    void setAudioOut(AudioOut * _ao);

private:
    void initializePeerConnection();
    void initializeDataChannel();
    void onDataChannel();
    int getCommand();
    void parseDescription(std::string descript);
    void parseCandidate(std::string candid);
    void connect_server();
    void setPeerConnectionCallbacks();
    void setDataChannelCallbacks();
};

#endif // WEBRTCOFERRER_H
