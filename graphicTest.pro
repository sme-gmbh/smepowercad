#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T16:47:52
#
#-------------------------------------------------

QT       += core gui opengl svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = graphicTest
TEMPLATE = app

LIBS += -lGLU


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
    geometryrenderengine.cpp \
    math/m3dbox.cpp \
    snapengine.cpp \
    glwidget.cpp \
    geometrydisplaytitle.cpp \
    items/cad_basic_point.cpp \
    items/cad_basic_line.cpp \
    items/cad_basic_polyline.cpp \
    items/cad_basic_circle.cpp \
    items/cad_basic_arc.cpp \
    items/cad_basic_3Dface.cpp \
    items/cad_basic_box.cpp \
    items/cad_basic_cylinder.cpp \
    items/cad_basic_sphere.cpp \
    items/cad_arch_door.cpp \
    items/cad_arch_levelslab.cpp \
    items/cad_arch_wall_loadbearing.cpp \
    items/cad_arch_wall_nonloadbearing.cpp \
    items/cad_arch_window.cpp \
    items/cad_basic_plane.cpp \
    items/cad_heatcool_pipe.cpp \
    items/cad_heatcool_chiller.cpp \
    items/cad_heatcool_coolingtower.cpp \
    items/cad_heatcool_heatexchanger.cpp \
    items/cad_heatcool_pump.cpp \
    items/cad_heatcool_controlvalve.cpp \
    items/cad_heatcool_adjustvalve.cpp \
    items/cad_heatcool_sensor.cpp \
    items/cad_air_duct.cpp \
    items/cad_air_pipe.cpp \
    items/cad_arch_blockout.cpp \
    items/cad_air_ductturn.cpp \
    items/cad_air_pipeturn.cpp \
    items/cad_air_pipereducer.cpp \
    items/cad_air_pipeteeconnector.cpp \
    items/cad_sprinkler_pipe.cpp \
    items/cad_sprinkler_head.cpp \
    items/cad_sprinkler_pump.cpp \
    items/cad_sprinkler_valve.cpp \
    items/cad_sprinkler_distribution.cpp \
    items/cad_sprinkler_teeconnector.cpp \
    items/cad_sprinkler_wetalarmvalve.cpp \
    items/cad_sprinkler_compressedairwatercontainer.cpp \
    items/cad_electrical_cabletray.cpp \
    items/cad_electrical_cabinet.cpp \
    items/cad_sprinkler_zonecheck.cpp

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
    geometryrenderengine.h \
    math/m3dbox.h \
    snapengine.h \
    glwidget.h \
    geometrydisplaytitle.h \
    items/cad_basic_point.h \
    items/cad_basic_line.h \
    items/cad_basic_polyline.h \
    items/cad_basic_arc.h \
    items/cad_basic_box.h \
    items/cad_basic_circle.h \
    items/cad_basic_cylinder.h \
    items/cad_basic_sphere.h \
    items/cad_basic_3Dface.h \
    items/cad_arch_door.h \
    items/cad_arch_levelslab.h \
    items/cad_arch_wall_loadbearing.h \
    items/cad_arch_wall_nonloadbearing.h \
    items/cad_arch_window.h \
    items/cad_basic_plane.h \
    items/cad_heatcool_pipe.h \
    items/cad_heatcool_chiller.h \
    items/cad_heatcool_coolingtower.h \
    items/cad_heatcool_heatexchanger.h \
    items/cad_heatcool_pump.h \
    items/cad_heatcool_controlvalve.h \
    items/cad_heatcool_adjustvalve.h \
    items/cad_heatcool_sensor.h \
    items/cad_air_duct.h \
    items/cad_air_pipe.h \
    items/cad_arch_blockout.h \
    items/cad_air_ductturn.h \
    items/cad_air_pipeturn.h \
    items/cad_air_pipereducer.h \
    items/cad_air_pipeteeconnector.h \
    items/cad_sprinkler_pipe.h \
    items/cad_sprinkler_head.h \
    items/cad_sprinkler_pump.h \
    items/cad_sprinkler_valve.h \
    items/cad_sprinkler_distribution.h \
    items/cad_sprinkler_teeconnector.h \
    items/cad_sprinkler_wetalarmvalve.h \
    items/cad_sprinkler_compressedairwatercontainer.h \
    items/cad_electrical_cabletray.h \
    items/cad_electrical_cabinet.h \
    items/cad_sprinkler_zonecheck.h

FORMS    += mainwindow.ui \
    layermanager.ui \
    geometrydisplaytitle.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    todo.txt
