QT = core
QT += network
CONFIG += c++17 cmdline

SOURCES += \
        cluster.cpp \
        controller.cpp \
        defs.cpp \
        host.cpp \
        main.cpp \
        meshcluster.cpp \
        node.cpp \
        packet.cpp \
        port.cpp \
        ringstarcluster.cpp \
        router.cpp \
        signaller.cpp

HEADERS += \
        cluster.hpp \
        meshcluster.hpp \
        ringstarcluster.hpp \
        router.hpp \
        controller.hpp \
        defs.hpp \
        host.hpp \
        node.hpp \
        packet.hpp \
        port.hpp \
        router.hpp \
        signaller.hpp \
        tableRow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
