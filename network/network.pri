INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include($$PWD/tcpserver/tcpserver.pri)

HEADERS += \
    $$PWD/clienthandler.h \
    $$PWD/server.h

SOURCES += \
    $$PWD/clienthandler.cpp \
    $$PWD/server.cpp
