INCLUDEPATH += $$PWD
DEPENDPATH *= $$PWD

QT += network

HEADERS += \
    $$PWD/tcpconnection.h \
    $$PWD/tcpconnections.h \
    $$PWD/tcpserver.h

SOURCES += \
    $$PWD/tcpconnection.cpp \
    $$PWD/tcpconnections.cpp \
    $$PWD/tcpserver.cpp


