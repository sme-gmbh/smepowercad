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
        Point,
        Line,
        Polyline,
        Circle,
        Arc,
        Face,
        Plane,
        Box,
        Cylinder,
        Sphere,
        LevelSlab,              // Decke
        Wall_loadBearing,       // Tagende Wand
        Wall_nonLoadBearing,    // Nicht tragende Wand
        BlockOut,
        Door,
        Window
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

private:
    ItemType type;
};

#endif // CADITEM_H
