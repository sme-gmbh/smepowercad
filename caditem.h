#ifndef CADITEM_H
#define CADITEM_H

#include <QString>
#include <QObject>
#include <QColor>
#include <QList>
#include <QMap>
#include <QMatrix4x4>
#include <QVector3D>
#include <QVariant>
#include <QImage>
#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QDebug>
#include <QOpenGLBuffer>
#include <math.h>
#include "math/m3dboundingbox.h"
#include "caditemtypes.h"

#define PI 3.1415926535897

class GLWidget;
class Layer;

class CADitem
{
public:
    CADitem(CADitemTypes::ItemType type);
    virtual ~CADitem() {}
    virtual QList<CADitemTypes::ItemType> flangable_items() = 0;
    virtual QImage wizardImage() = 0;
    virtual QString iconPath() = 0;
    virtual QString domain() = 0;
    virtual QString description() = 0;
    virtual void calculate() = 0;
    virtual void processWizardInput() = 0;
    virtual void paint(GLWidget* glwidget) {Q_UNUSED(glwidget)}
    QColor getColorPen();
    QColor getColorBrush();
    void setLayer(Layer* layer);
    void setID(quint64 id);
    void serialOut(QByteArray *out);
    bool serialIn(QByteArray *in);

    // data types tbd.
    CADitemTypes::ItemType getType();
    Layer* layer;
    QColor color_pen;       // Transparent means "BYLAYER"
    QColor color_brush;       // Transparent means "BYLAYER"
    QMap<QString, QString> attributes;
    M3dBoundingBox boundingBox;
    QList<CADitem*> subItems;
    quint64 id;

    QVector3D position;
    qreal angle_x;
    qreal angle_y;
    qreal angle_z;
    QMatrix4x4 matrix_rotation;

    // Wizard
    QMap<QString,QVariant> wizardParams;

    // Object Snap
    QList<QVector3D> snap_center;
    QList<QVector3D> snap_vertices;
    QList<QVector3D> snap_flanges;
    QVector3D snap_basepoint;

    // Object Editing

    // Highlighting
    bool highlight; // Highlight shows that the mouse is over the object
    quint32 index;

    // Selection
    bool selected;  // Selection shows that the item is selected as an operand for commands

private:
    void setLayer_processItems(QList<CADitem*> subItems);
    void setID_processItems(QList<CADitem*> subItems);
    CADitemTypes::ItemType type;
};


#endif // CADITEM_H
