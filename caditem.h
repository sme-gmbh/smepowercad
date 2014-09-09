#ifndef CADITEM_H
#define CADITEM_H

#include <QString>
#include <QColor>
#include <QList>
#include <QMap>
#include <QVector3D>
#include "math/m3dbox.h"

class CADitem
{
public:
    enum ItemType
    {
        Basic_Point,
        Basic_Line,
        Basic_Polyline,
        Basic_Circle,
        Basic_Arc,
        Basic_Face,
        Basic_Plane,
        Basic_Box,
        Basic_Cylinder,
        Basic_Sphere,
        Arch_LevelSlab,                         // Decke
        Arch_Wall_loadBearing,                  // Tagende Wand
        Arch_Wall_nonLoadBearing,               // Nicht tragende Wand
        Arch_BlockOut,
        Arch_Door,
        Arch_Window,
        Air_Duct,                               // Kanal (eckig)
        Air_Pipe,                               // Rohr (rund)
        Air_DuctFireResistant,                  // Kanal (feuerbeständig)
        Air_DuctTurn,                           // Kanalbogen
        Air_PipeTurn,                           // Rohrbogen
        Air_PipeReducer,                        // Rohrreduzierung
        Air_PipeTeeConnector,                   // Rohr-T-Stück, auch mit mehr als 3 Anschlüssen
        Air_DuctTeeConnector,                   // Kanal-T-Stück
        Air_DuctTransition,                     // Kanalübergang (eckig)
        Air_DuctTransitionRectRound,            // Kanalübergang eckig/rund
        Air_DuctYpiece,                         // Kanal-Hosenstück
        Air_DuctEndPlate,                       // Kanalenddeckel (Boden)
        Air_PipeEndCap,                         // Rohr-Enddeckel
        Air_ThrottleValve,                      // Drossel-/ Absperrklappe (rund)
        Air_MultiLeafDamper,                    // Jalousieklappe (eckig)
        Air_PressureReliefDamper,               // Druckentlastungsklappe
        Air_PipeFireDamper,                     // Brandschutzklappe (rund)
        Air_DuctFireDamper,                     // Brandschutzklappe (eckig)
        Air_DuctVolumetricFlowController,       // Kanal-Volumenstromregler
        Air_PipeVolumetricFlowController,       // Rohr-Volumenstromregler
        Air_HeatExchangerWaterAir,              // Wärmetauscher Wasser/Luft
        Air_HeatExchangerAirAir,                // Wärmetauscher Luft/Luft
        Air_CanvasFlange,                       // Segeltuchstutzen
        Air_Filter,                             // Luftfilter
        Air_PipeSilencer,                       // Rohr-Schalldämpfer
        Air_DuctBaffleSilencer,                 // Kanal-Kulissenschalldämpfer
        Air_Fan,                                // Ventilator
        Air_Humidifier,                         // Befeuchter
        Air_EmptyCabinet,                       // Leergerät
        Air_EquipmentFrame,                     // Geräterahmen
        HeatCool_Pipe,                          // Rohr
        HeatCool_Chiller,                       // Kältemaschine
        HeatCool_CoolingTower,                  // Kühlturm
        HeatCool_ExpansionChamber,              // Ausdehnungsgefäß
        HeatCool_Boiler,                        // Heizkessel
        HeatCool_WaterHeater,                   // Warmwasserboiler
        HeatCool_StorageBoiler,                 // Speicherkessel / Schichtspeicher
        HeatCool_HeatExchanger,                 // Wärmetauscher
        HeatCool_Pump,                          // Pumpe
        HeatCool_Controlvalve,                  // Regelventil
        HeatCool_Adjustvalve,                   // Regulierventil
        HeatCool_Sensor,                        // Sensor
        Sprinkler_Pipe,
        Sprinkler_Head,
        Sprinkler_Pump,
        Sprinkler_Valve,
        Sprinkler_Distribution,
        Sprinkler_TeeConnector,
        Sprinkler_ZoneCheck,
        Sprinkler_WetAlarmValve,
        Sprinkler_CompressedAirWaterContainer,
        Electrical_CableTray,
        Electrical_Cabinet


    };

    CADitem(ItemType type);
    void calculate();

    // data types tbd.
    ItemType getType();
    QString layer;
    QColor color_pen;       // Transparent means "BYLAYER"
    QColor color_brush;       // Transparent means "BYLAYER"
    QMap<QString, QString> attributes;
    QVector3D position;
    M3dBox boundingBox;
    QList<CADitem*> subItems;

    // Object Snap
    QVector3D snap_center;
    QList<QVector3D> snap_vertices;

    // Highlighting
    bool highlight;
    quint32 index;

private:
    ItemType type;
};

#endif // CADITEM_H
