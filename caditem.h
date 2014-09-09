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
        Arch_LevelSlab,              // Decke
        Arch_Wall_loadBearing,       // Tagende Wand
        Arch_Wall_nonLoadBearing,    // Nicht tragende Wand
        Arch_BlockOut,
        Arch_Door,
        Arch_Window,
        Air_Duct,
        Air_Pipe,
        Air_DuctTurn,
        Air_PipeTurn,
        Air_PipeReducer,
        Air_TeeConnector,
        HeatCool_Pipe,
        HeatCool_Chiller,
        HeatCool_CoolingTower,
        HeatCool_HeatExchanger,
        HeatCool_Pump,
        HeatCool_Controlvalve,
        HeatCool_Adjustvalve,
        HeatCool_Sensor,
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
        Electrical_Cabinet,


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
