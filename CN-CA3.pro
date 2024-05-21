QT = core
QT +=
CONFIG += c++17 cmdline

SOURCES += \
        controller.cpp \
        host.cpp \
        main.cpp \
        packet.cpp \
        router.cpp \
        wire.cpp

HEADERS += \
        Router.hpp \
        controller.hpp \
        forwardingRow.h \
        host.hpp \
        packet.hpp \
        router.hpp \
        wire.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
