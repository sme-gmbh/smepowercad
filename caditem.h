/**********************************************************************
** smepowercad
** Copyright (C) 2015 Smart Micro Engineering GmbH
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
** You should have received a copy of the GNU General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

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
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <math.h>
#include "math/m3dboundingbox.h"
#include "math/manglecalculations.h"
#include "math/mtriangle.h"
#include "caditemtypes.h"
#include "wizardparams.h"

#define PI 3.1415926535897

class GLWidget;
class Layer;

class CADitem
{
public:
    CADitem(CADitemTypes::ItemType type);
    virtual ~CADitem() {}
    virtual QList<CADitemTypes::ItemType> flangable_items(int flangeIndex) = 0;
    virtual QImage wizardImage() = 0;
    virtual QString iconPath() = 0;
    virtual QString domain() = 0;
    virtual QString description() = 0;
    virtual void calculate() = 0;
    virtual void processWizardInput() = 0;
    virtual void paint(GLWidget* glwidget) {Q_UNUSED(glwidget)}
    virtual QList<MTriangle> getTriangles();
    //Flanging
    virtual QMatrix4x4 rotationOfFlange(quint8 num) {Q_UNUSED(num); return QMatrix4x4();}
    QColor getColorPen();
    QColor getColorBrush();
    void setLayer(Layer* layer);
    void setFormerLayer(Layer *layer);
    void setID(quint64 id);
    void serialOut(QByteArray *out);
    bool serialIn(QByteArray *in);
    void rotateAroundAxis(qreal angle, QVector3D axis, qreal angle_x, qreal angle_y, qreal angle_z);
    WizardParams rotateAroundPoint(QVector3D center, qreal angle_x, qreal angle_y, qreal angle_z);

    // data types tbd.
    CADitemTypes::ItemType getType();
    Layer* layer;
    Layer* formerLayer;
    QColor color_pen;       // Transparent means "BYLAYER"
    QColor color_brush;       // Transparent means "BYLAYER"
//    QMap<QString, QString> attributes;
    M3dBoundingBox boundingBox;
    QList<CADitem*> subItems;
    quint64 id;

    QVector3D position;
    qreal angle_x;
    qreal angle_y;
    qreal angle_z;
    QMatrix4x4 matrix_rotation;

    // Wizard
    WizardParams wizardParams;

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

    // Painting
    bool isMaintenanceArea;

    // OpenGL Buffers
    QOpenGLBuffer* arrayBufVertices;
    QOpenGLBuffer* indexBufFaces;
    QOpenGLBuffer* indexBufLines;

private:
    void setLayer_processItems(QList<CADitem*> subItems);
    void setFormerLayer_processItems(QList<CADitem *> subItems);
    void setID_processItems(QList<CADitem*> subItems);
    QList<MTriangle> triangleListFromIndexedBuffers();
    CADitemTypes::ItemType type;

};


#endif // CADITEM_H
