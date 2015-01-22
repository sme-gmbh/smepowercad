#-------------------------------------------------
#
# Project created by QtCreator 2013-12-03T16:47:52
#
#-------------------------------------------------

QT       += core gui opengl svg xml network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
#greaterThan(QT_MAJOR_VERSION, 4): QT -= opengl

TARGET = graphicTest
TEMPLATE = app

OBJECTS_DIR = .obj/
MOC_DIR = .moc/
UI_DIR = .ui/
RCC_DIR = .rcc/

LIBS += -lGLU -lX11

TRANSLATIONS =  lang/powercad-de_DE.ts \
                lang/powercad-ru_RU.ts

SOURCES += main.cpp\
        mainwindow.cpp \
    layer.cpp \
    layermanager.cpp \
    cadcommand.cpp \
    creationinterface.cpp \
    dxflib/src/dl_writer_ascii.cpp \
    dxflib/src/dl_dxf.cpp \
    geometrydisplay.cpp \
    itemdb.cpp \
    caditem.cpp \
    math/m3dbox.cpp \
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
    items/cad_sprinkler_zonecheck.cpp \
    items/cad_air_ductfireresistant.cpp \
    items/cad_air_ductteeconnector.cpp \
    items/cad_air_ducttransition.cpp \
    items/cad_air_ducttransitionrectround.cpp \
    items/cad_air_ductypiece.cpp \
    items/cad_air_ductendplate.cpp \
    items/cad_air_throttlevalve.cpp \
    items/cad_air_multileafdamper.cpp \
    items/cad_air_pressurereliefdamper.cpp \
    items/cad_air_pipefiredamper.cpp \
    items/cad_air_ductfiredamper.cpp \
    items/cad_air_pipevolumetricflowcontroller.cpp \
    items/cad_air_heatexchangerwaterair.cpp \
    items/cad_air_heatexchangerairair.cpp \
    items/cad_air_canvasflange.cpp \
    items/cad_air_filter.cpp \
    items/cad_air_pipesilencer.cpp \
    items/cad_air_ductbafflesilencer.cpp \
    items/cad_air_fan.cpp \
    items/cad_air_humidifier.cpp \
    items/cad_air_emptycabinet.cpp \
    items/cad_air_equipmentframe.cpp \
    items/cad_air_pipeendcap.cpp \
    items/cad_heatcool_expansionchamber.cpp \
    items/cad_heatcool_boiler.cpp \
    items/cad_heatcool_waterheater.cpp \
    items/cad_heatcool_storageboiler.cpp \
    items/cad_air_ductvolumetricflowcontroller.cpp \
    items/cad_heatcool_pipeturn.cpp \
    items/cad_heatcool_pipereducer.cpp \
    items/cad_heatcool_pipeteeconnector.cpp \
    items/cad_heatcool_pipeendcap.cpp \
    items/cad_heatcool_flange.cpp \
    items/cad_heatcool_filter.cpp \
    items/cad_heatcool_ballvalve.cpp \
    items/cad_heatcool_butterflyvalve.cpp \
    items/cad_heatcool_safetyvalve.cpp \
    items/cad_arch_support.cpp \
    items/cad_arch_beam.cpp \
    items/cad_heatcool_radiator.cpp \
    items/cad_heatcool_flowmeter.cpp \
    modaldialog.cpp \
    settingsdialog.cpp \
    3Dmouse/qmagellan.cpp \
    3Dmouse/xdrvlib.c \
    itemwizard.cpp \
    items/cad_sprinkler_pipeturn.cpp \
    items/cad_sprinkler_pipereducer.cpp \
    items/cad_sprinkler_pipeendcap.cpp \
    items/cad_sanitary_washbasin.cpp \
    items/cad_sanitary_sink.cpp \
    items/cad_sanitary_shower.cpp \
    items/cad_sanitary_pipeturn.cpp \
    items/cad_sanitary_pipeteeconnector.cpp \
    items/cad_sanitary_pipereducer.cpp \
    items/cad_sanitary_pipeendcap.cpp \
    items/cad_sanitary_pipe.cpp \
    items/cad_sanitary_liftingunit.cpp \
    items/cad_sanitary_flange.cpp \
    items/cad_sanitary_emergencyshower.cpp \
    items/cad_sanitary_emergencyeyeshower.cpp \
    items/cad_sanitary_electricwaterheater.cpp \
    items/cad_basic_pipe.cpp \
    items/cad_basic_turn.cpp \
    math/m3dboundingbox.cpp \
    network/server.cpp \
    network/clienthandler.cpp \
    items/cad_basic_duct.cpp \
    items/cad_arch_boredPile.cpp \
    items/cad_arch_grating.cpp \
    items/cad_arch_foundation.cpp \
    items/cad_basic_face.cpp \
    itemgripmodifier.cpp \
    items/cad_air_pipebranch.cpp \
    toolwidget.cpp \
    caditemtypes.cpp \
    wizardparams.cpp

HEADERS  += mainwindow.h \
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
    math/m3dbox.h \
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
    items/cad_sprinkler_zonecheck.h \
    items/cad_air_ductfireresistant.h \
    items/cad_air_ductteeconnector.h \
    items/cad_air_ducttransition.h \
    items/cad_air_ducttransitionrectround.h \
    items/cad_air_ductypiece.h \
    items/cad_air_ductendplate.h \
    items/cad_air_throttlevalve.h \
    items/cad_air_multileafdamper.h \
    items/cad_air_pressurereliefdamper.h \
    items/cad_air_pipefiredamper.h \
    items/cad_air_ductfiredamper.h \
    items/cad_air_pipevolumetricflowcontroller.h \
    items/cad_air_heatexchangerwaterair.h \
    items/cad_air_heatexchangerairair.h \
    items/cad_air_canvasflange.h \
    items/cad_air_filter.h \
    items/cad_air_pipesilencer.h \
    items/cad_air_ductbafflesilencer.h \
    items/cad_air_fan.h \
    items/cad_air_humidifier.h \
    items/cad_air_emptycabinet.h \
    items/cad_air_equipmentframe.h \
    items/cad_air_pipeendcap.h \
    items/cad_heatcool_expansionchamber.h \
    items/cad_heatcool_boiler.h \
    items/cad_heatcool_waterheater.h \
    items/cad_heatcool_storageboiler.h \
    items/cad_air_ductvolumetricflowcontroller.h \
    items/cad_heatcool_pipeturn.h \
    items/cad_heatcool_pipereducer.h \
    items/cad_heatcool_pipeteeconnector.h \
    items/cad_heatcool_pipeendcap.h \
    items/cad_heatcool_flange.h \
    items/cad_heatcool_filter.h \
    items/cad_heatcool_ballvalve.h \
    items/cad_heatcool_butterflyvalve.h \
    items/cad_heatcool_safetyvalve.h \
    items/cad_arch_support.h \
    items/cad_arch_beam.h \
    items/cad_heatcool_radiator.h \
    items/cad_heatcool_flowmeter.h \
    modaldialog.h \
    settingsdialog.h \
    3Dmouse/qmagellan.h \
    3Dmouse/xdrvlib.h \
    itemwizard.h \
    items/cad_sprinkler_pipeturn.h \
    items/cad_sprinkler_pipereducer.h \
    items/cad_sprinkler_pipeendcap.h \
    items/cad_sanitary_washbasin.h \
    items/cad_sanitary_sink.h \
    items/cad_sanitary_shower.h \
    items/cad_sanitary_pipeturn.h \
    items/cad_sanitary_pipeteeconnector.h \
    items/cad_sanitary_pipereducer.h \
    items/cad_sanitary_pipeendcap.h \
    items/cad_sanitary_pipe.h \
    items/cad_sanitary_liftingunit.h \
    items/cad_sanitary_flange.h \
    items/cad_sanitary_emergencyshower.h \
    items/cad_sanitary_emergencyeyeshower.h \
    items/cad_sanitary_electricwaterheater.h \
    items/cad_basic_pipe.h \
    items/cad_basic_turn.h \
    math/m3dboundingbox.h \
    network/server.h \
    network/clienthandler.h \
    items/cad_basic_duct.h \
    items/cad_arch_boredPile.h \
    items/cad_arch_grating.h \
    items/cad_arch_foundation.cpp, cad_arch_foundation.h \
    items/cad_arch_foundation.h \
    items/cad_basic_face.h \
    itemgripmodifier.h \
    items/cad_air_pipebranch.h \
    toolwidget.h \
    caditemheaderincludes.h \
    caditemtypes.h \
    wizardparams.h

FORMS    += mainwindow.ui \
    layermanager.ui \
    geometrydisplaytitle.ui \
    modaldialog.ui \
    settingsdialog.ui \
    itemwizard.ui \
    itemgripmodifier.ui \
    toolwidget.ui

RESOURCES += \
    icons.qrc

OTHER_FILES += \
    todo.txt \
    lang/powercad-de_DE.ts \
    lang/powercad-ru_RU.ts \
    lang/powercad-de_DE.qm \
    lang/powercad-ru_RU.qm \
    settings.xml \
    shaders/test.frag \
    shaders/shader_1.vert \
    shaders/shader_1_triangles.geom \
    shaders/shader_1_lines.geom \
    shaders/shader_2.frag \
    shaders/shader_2.vert
