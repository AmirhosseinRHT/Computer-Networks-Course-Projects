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

void WebRTCClientAnswerer::start() {
    onDataChannel();
    std::string config = TCPserver.recieveMessage();
    pc->setRemoteDescription(config);
    std::cout << "cand size:\n" << cand.size() << std::endl;
    TCPserver.sendMessage(desc);
    for (int i = 0 ; i < qMin((int) cand.size() , 1) ; i++)
        TCPserver.sendMessage(cand[i]);
    //handleCommands();
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

void WebRTCClientAnswerer::handleCommands() {
    bool exit = false;
    int command = getCommand();
    while (!exit) {
        printMenu();
        // int command = getCommand();
        executeCommand(command, exit);
    }
}

void WebRTCClientAnswerer::printMenu() {
    std::cout << std::endl
              << "**********************************************************************************"
                 "*****"
              << std::endl
              << "* 0: Exit / 1: Enter remote description / 2: Enter remote candidate / 3: Send message / 4: Print Connection Info *" << std::endl
              << "[Command]: ";
}

int WebRTCClientAnswerer::getCommand() {

    std::string config = TCPserver.recieveMessage();
    pc->setRemoteDescription(config);
    std::cout << "cand size:\n" << cand.size();
    TCPserver.sendMessage(desc);
    for (int i = 0 ; i < cand.size() ; i++){
        TCPserver.sendMessage(cand[i]);
    }
        int command = -1;
    std::cin >> command;
    std::cin.ignore();
    return command;
}

void WebRTCClientAnswerer::executeCommand(int command, bool& exit) {
    //sendMessage();
}

void WebRTCClientAnswerer::parseDescription() {
    std::cout << "[Description]: ";
    std::string sdp, line;
    while (getline(std::cin, line) && !line.empty()) {
        sdp += line;
        sdp += "\r\n";
    }
    pc->setRemoteDescription(sdp);
}

void WebRTCClientAnswerer::parseCandidate() {
    std::cout << "[Candidate]: ";
    std::string candidate;
    getline(std::cin, candidate);
    pc->addRemoteCandidate(candidate);
}

void WebRTCClientAnswerer::sendMessage(const char * mess) {
    if (!dc->isOpen()) {
        std::cout << "** Channel is not Open ** ";
        return;
    }
    //std::cout << "[Message]: ";
    std::string message = (std::string) mess;
    //getline(std::cin, message);
    dc->send(message);
}

void WebRTCClientAnswerer::printConnectionInfo() {
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



void WebRTCClientAnswerer::setPeerConnectionCallbacks() {
    pc->onLocalDescription([this](rtc::Description description) {
        // std::cout << "Local Description22 (Paste this to the other peer):" << std::endl;
        // std::cout << std::string(description) << std::endl;
        desc = std::string(description);
            // std::cout <<"myfvbbbb desc:" << desc << std::endl;
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

void WebRTCClientAnswerer::setDataChannelCallbacks() {
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

            }
        }


        if(ao){
            QByteArray buffer;
            // buffer.setRawData()

        }
    });



}


void WebRTCClientAnswerer::setAudioOut(AudioOut *_ao){
    ao = _ao;
}
