#include "webRtcOferrer.h"

WebRTCClientOferrer::WebRTCClientOferrer() {
    std::cout << "oferrer   ----   client\n ";
    rtc::InitLogger(rtc::LogLevel::Warning);
    initializePeerConnection();
    role = 1;
}

WebRTCClientOferrer::WebRTCClientOferrer(std::string _ip){
    std::cout << "oferrer   ----   client\n ";
    rtc::InitLogger(rtc::LogLevel::Warning);
    ip = _ip;
    initializePeerConnection();
    role = 1;
}

void WebRTCClientOferrer::start() {
    initializeDataChannel();
    connect_server();
}

void WebRTCClientOferrer::close() {
    if (dc)
        dc->close();
    if (pc)
        pc->close();
}

void WebRTCClientOferrer::initializePeerConnection() {

    rtc::Configuration config;
    config.iceServers.emplace_back("stun.l.google.com:19302");
    pc = std::make_shared<rtc::PeerConnection>(config);
    setPeerConnectionCallbacks();
}

void WebRTCClientOferrer::initializeDataChannel() {
    dc = pc->createDataChannel("test");
    setDataChannelCallbacks();
    std::this_thread::sleep_for(1s);
}

void WebRTCClientOferrer::onDataChannel() {
    pc->onDataChannel([&](shared_ptr<rtc::DataChannel> _dc) {
        dc = _dc;
        setDataChannelCallbacks();
    });
}

void WebRTCClientOferrer::connect_server(){
    TCPClient client(ip, 12345);
    client.Connect();
    client.SendMessage(desc);
    parseDescription(client.recieveMessage());
    parseCandidate(client.recieveMessage());
}


int WebRTCClientOferrer::getCommand() {
    int command = -1;
    std::cin >> command;
    std::cin.ignore();
    return command;
}

void WebRTCClientOferrer::parseDescription(std::string descript) {
    std::cout << "[Description]: ";
    pc->setRemoteDescription(descript);
}

void WebRTCClientOferrer::parseCandidate(std::string candid) {
    std::cout << "[Candidate]: ";
    pc->addRemoteCandidate(candid);
}

void WebRTCClientOferrer::sendMessage(const char * mess) {
    if (!dc->isOpen()) {
        std::cout << "Channel is not Open\n";
        return;
    }
    std::string message = (std::string) mess;
    dc->send(message);
}

void WebRTCClientOferrer::setPeerConnectionCallbacks() {
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

void WebRTCClientOferrer::setDataChannelCallbacks() {
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

void WebRTCClientOferrer::setAudioOut(AudioOut *_ao){
    ao = _ao;
}
