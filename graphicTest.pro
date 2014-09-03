#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T16:47:52
#
#-------------------------------------------------

QT       += core gui opengl svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphicTest
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    overlay.cpp \
    layer.cpp \
    layermanager.cpp \
    cadcommand.cpp \
    creationinterface.cpp \
    dxflib/src/dl_writer_ascii.cpp \
    dxflib/src/dl_dxf.cpp \
    geometrydisplay.cpp \
    itemdb.cpp \
    caditem.cpp \
    items/cadline.cpp \
    items/cadpolyline.cpp \
    geometryrenderengine.cpp \
    math/m3dbox.cpp \
    snapengine.cpp \
    items/cad3Dface.cpp \
    glwidget.cpp \
    geometrydisplaytitle.cpp

HEADERS  += mainwindow.h \
    overlay.h \
    layer.h \
    layermanager.h \
    cadcommand.h \
    creationinterface.h \
    dxflib/src/dl_writer_ascii.h \
    dxflib/src/dl_writer.h \
    dxflib/src/dl_global.h \
    dxflib/src/dl_extrusion.h \
    dxflib/src/dl_exception.h \
    dxflib/src/dl_entities.h \
    dxflib/src/dl_dxf.h \
    dxflib/src/dl_creationinterface.h \
    dxflib/src/dl_creationadapter.h \
    dxflib/src/dl_codes.h \
    dxflib/src/dl_attributes.h \
    geometrydisplay.h \
    itemdb.h \
    caditem.h \
    items/cadline.h \
    items/cadpolyline.h \
    geometryrenderengine.h \
    math/m3dbox.h \
    snapengine.h \
    items/cad3Dface.h \
    glwidget.h \
    geometrydisplaytitle.h

FORMS    += mainwindow.ui \
    layermanager.ui \
    geometrydisplaytitle.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    todo.txt
