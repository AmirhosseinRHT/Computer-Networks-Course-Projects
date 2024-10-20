#include "WebRTCAnswerer.h"

#include<QByteArray>
#include<string>

WebRTCClientAnswerer::WebRTCClientAnswerer() {
    std::cout << "answerer ----- server\n";
    rtc::InitLogger(rtc::LogLevel::Warning);
    initializePeerConnection();
    TCPserver.setPort(12345);
    TCPserver.Start();
    role = 2;
}

WebRTCClientAnswerer::WebRTCClientAnswerer(std::string _ip){
    std::cout << "answerer ----- server\n";
    rtc::InitLogger(rtc::LogLevel::Warning);
    initializePeerConnection();
    TCPserver.setPort(12345);
    TCPserver.Start();
    role = 2;
    ip = _ip;
}

void WebRTCClientAnswerer::start() {
    onDataChannel();
    std::string config = TCPserver.recieveMessage();
    pc->setRemoteDescription(config);
    TCPserver.sendMessage(desc);
    TCPserver.sendMessage(cand[cand.size()-1]);
}

void WebRTCClientAnswerer::close() {
    if (dc)
        dc->close();
    if (pc)
        pc->close();
}

void WebRTCClientAnswerer::initializePeerConnection() {
    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");
    pc = std::make_shared<rtc::PeerConnection>(config);
    setPeerConnectionCallbacks();

}

void WebRTCClientAnswerer::initializeDataChannel() {
    dc = pc->createDataChannel("test");
    setDataChannelCallbacks();
    std::this_thread::sleep_for(1s);
}

void WebRTCClientAnswerer::onDataChannel() {
    pc->onDataChannel([&](shared_ptr<rtc::DataChannel> _dc) {
        dc = _dc;
        setDataChannelCallbacks();
    });
}

void WebRTCClientAnswerer::parseCandidate() {
    std::cout << "[Candidate]: ";
    std::string candidate;
    getline(std::cin, candidate);
    pc->addRemoteCandidate(candidate);
}

void WebRTCClientAnswerer::sendMessage(const char * mess) {
    if (!dc->isOpen()) {
        std::cout << "Channel is not Open \n ";
        return;
    }
    std::string message = (std::string) mess;
    dc->send(message);
}

void WebRTCClientAnswerer::setPeerConnectionCallbacks() {
    pc->onLocalDescription([this](rtc::Description description) {
        desc = std::string(description);
    });

    pc->onLocalCandidate([this](rtc::Candidate candidate) {
        cand.push_back(std::string(candidate));
    });

    pc->onStateChange([](rtc::PeerConnection::State state) {
        std::cout << "[State: " << state << "]" << std::endl;
    });

    pc->onGatheringStateChange([](rtc::PeerConnection::GatheringState state) {
        std::cout << "[Gathering State: " << state << "]" << std::endl;
    });
}

void WebRTCClientAnswerer::setDataChannelCallbacks() {
    dc->onOpen([&]() {
        std::cout << "[DataChannel open: " << dc->label() << "]" << std::endl;
    });

    dc->onClosed([&]() {
        std::cout << "[DataChannel closed: " << dc->label() << "]" << std::endl;
    });

    dc->onMessage([this](auto data) {
        if (std::holds_alternative<std::string>(data)) {
            std::string d = std::get<std::string>(data);
            if(ao){
                QByteArray buffer(d.size() , 0);
                buffer.setRawData(d.c_str() , d.size());
                ao->play(buffer);
            }
        }
    });
}


void WebRTCClientAnswerer::setAudioOut(AudioOut *_ao){
    ao = _ao;
}
