QT = core
QT += network
QT += concurrent
CONFIG += c++17 cmdline

SOURCES += \
        cluster.cpp \
        commandreader.cpp \
        defs.cpp \
        host.cpp \
        main.cpp \
        meshcluster.cpp \
        network.cpp \
        node.cpp \
        packet.cpp \
        parto.cpp \
        port.cpp \
        ringstarcluster.cpp \
        router.cpp \
        signaller.cpp \
        toruscluster.cpp

HEADERS += \
        cluster.hpp \
        commandreader.hpp \
        meshcluster.hpp \
        parto.hpp \
        network.hpp \
        ringstarcluster.hpp \
        router.hpp \
        defs.hpp \
        host.hpp \
        node.hpp \
        packet.hpp \
        port.hpp \
        router.hpp \
        signaller.hpp \
        tableRow.h \
        toruscluster.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
