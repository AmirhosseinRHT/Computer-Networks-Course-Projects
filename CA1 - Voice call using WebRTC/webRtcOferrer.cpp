#include "webRtcOferrer.h"

WebRTCClientOferrer::WebRTCClientOferrer() {
    std::cout << "oferrer   ----   client\n ";
    rtc::InitLogger(rtc::LogLevel::Warning);

    initializePeerConnection();
    role = 1;
}

void WebRTCClientOferrer::start() {
    initializeDataChannel();
    connect_server();
    //handleCommands();
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
    bool exit = false;
    TCPClient client("127.0.0.1", 12345);
    client.Connect();
    client.SendMessage(desc);
    parseDescription(client.recieveMessage());
    parseCandidate(client.recieveMessage());
}

void WebRTCClientOferrer::handleCommands() {
    bool exit = false;
    // TCPClient client("127.0.0.1", 12345);
    // client.Connect();
    // client.SendMessage(desc);
    // parseDescription(client.recieveMessage());
    // parseCandidate(client.recieveMessage());
    int command = getCommand();
    while (!exit) {
        printMenu();
        // int command = getCommand();
        executeCommand(command, exit);
    }
}

void WebRTCClientOferrer::printMenu() {
    std::cout << std::endl
              << "**********************************************************************************"
                 "*****"
              << std::endl
              << "* 0: Exit / 1: Enter remote description / 2: Enter remote candidate / 3: Send message / 4: Print Connection Info *" << std::endl
              << "[Command]: ";
}

int WebRTCClientOferrer::getCommand() {
    int command = -1;
    std::cin >> command;
    std::cin.ignore();
    return command;
}

void WebRTCClientOferrer::executeCommand(int command, bool& exit) {
    //sendMessage();
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
        std::cout << "** Channel is not Open ** ";
        return;
    }
    //std::cout << "[Message]: ";
    std::string message = (std::string) mess;
    //getline(std::cin, message);
    dc->send(message);
}

void WebRTCClientOferrer::printConnectionInfo() {
    if (!dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }
    rtc::Candidate local, remote;
    std::optional<std::chrono::milliseconds> rtt = pc->rtt();
    if (pc->getSelectedCandidatePair(&local, &remote)) {
        std::cout << "Local: " << local << std::endl;
        std::cout << "Remote: " << remote << std::endl;
        std::cout << "Bytes Sent:" << pc->bytesSent() << " / Bytes Received:" << pc->bytesReceived();
        if (rtt.has_value())
            std::cout << " / Round-Trip Time:" << rtt.value().count() << " ms";
        else
            std::cout << " / Round-Trip Time: null ms";
    } else {
        std::cout << "Could not get Candidate Pair Info" << std::endl;
    }
}

void WebRTCClientOferrer::setPeerConnectionCallbacks() {
    pc->onLocalDescription([this](rtc::Description description) {
        // std::cout << "Local Description2 (Paste this to the other peer):" << std::endl;
        desc = std::string(description);
        // std::cout <<"myfgggggggggggggg desc:" << desc << std::endl;
    });

    pc->onLocalCandidate([this](rtc::Candidate candidate) {
        // std::cout << "Local Candidate (Paste this to the other peer after the local description):" << std::endl;
        // std::cout << std::string(candidate) << std::endl << std::endl;
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
            std::cout << "[Received: " << std::get<std::string>(data) << "]" << std::endl;
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
