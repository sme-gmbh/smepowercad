INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include($$PWD/tcpserver/tcpserver.pri)

HEADERS += \
    $$PWD/clienthandler.h \
    $$PWD/server.h \
    $$PWD/spcpconnection.h

SOURCES += \
    $$PWD/clienthandler.cpp \
    $$PWD/server.cpp \
    $$PWD/spcpconnection.cpp
