QT = core
QT +=
CONFIG += c++17 cmdline

SOURCES += \
        cluster.cpp \
        controller.cpp \
        host.cpp \
        main.cpp \
        node.cpp \
        packet.cpp \
        router.cpp

HEADERS += \
        Router.hpp \
        cluster.hpp \
        controller.hpp \
        defs.hpp \
        host.hpp \
        node.hpp \
        packet.hpp \
        router.hpp \
        tableRow.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
