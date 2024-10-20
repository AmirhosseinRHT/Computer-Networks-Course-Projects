QT = core
QT += multimedia widgets
QT += network
QT += websockets
CONFIG += c++17 cmdline


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Audio.cpp \
        AudioOut.cpp \
        WebRTCAnswerer.cpp \
        main.cpp \
        webRtcOferrer.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH += C:\Users\Amir\Desktop\CN_A1\boost_1_84_0
LIBS += -LC:\Users\Amir\Desktop\CN_A1\boost_1_84_0\stage\mingw-x64\lib -lboost_system-mgw11-mt-d-x64-1_84
LIBS += -lws2_32


INCLUDEPATH += C:\Users\Amir\Desktop\CN_A1\libdatachannel\include
LIBS += -LC:\Users\Amir\Desktop\CN_A1\libdatachannel\build -ldatachannel.dll
LIBS += -LC:\Qt\Tools\OpenSSLv3\Win_x64\bin -lcrypto-3-x64 -lssl-3-x64


# HEADERS += Audio.h

HEADERS += \
    Audio.h \
    AudioOut.h \
    TCPClient.h \
    TCPServer.h \
    WebRTCAnswerer.h \
    webRtcOferrer.h

