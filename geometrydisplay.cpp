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

#include "geometrydisplay.h"

#include <QDebug>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
//#include <QGLFormat>
#include <qmath.h>

GeometryDisplay::GeometryDisplay(ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier *itemGripModifier, QWidget *parent) :
    QDockWidget(parent)
{
//    qDebug() << "GeometryDisplay created";
    this->itemDB = itemDB;
    this->itemWizard = itemWizard;
    this->itemGripModifier = itemGripModifier;

    titleWidget = new GeometryDisplayTitle();
    connect(this, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), titleWidget, SLOT(slot_sceneCoordinatesChanged(QVector3D)));
    connect(titleWidget, SIGNAL(signal_float()), this, SLOT(slot_float()));
    connect(titleWidget, SIGNAL(signal_close()), this, SLOT(slot_close()));
    connect(titleWidget, SIGNAL(signal_cuttingplane_changed(QString)), this, SLOT(slot_changeCuttingplane(QString)));
    this->setTitleBarWidget(titleWidget);

    this->setWindowTitle(tr("Drawing %1").arg("x"));


//    QGLFormat glFormat;
//    glFormat.setVersion(4, 3);
//    glFormat.setProfile(QGLFormat::CompatibilityProfile);
//    glFormat.setSampleBuffers(true);
//    glFormat.setAlphaBufferSize(8);

    glwidget = new GLWidget(this, itemDB, itemWizard, itemGripModifier);
//    glwidget = new GLWidget(this, itemDB, itemWizard);
    this->setWidget(glwidget);
    connect(this, SIGNAL(signal_repaintNeeded()), glwidget, SLOT(slot_repaint()));
    connect(this, SIGNAL(signal_itemDeleted(CADitem*)), glwidget, SLOT(slot_itemDeleted(CADitem*)));
    connect(titleWidget, SIGNAL(signal_wireframe(bool)), glwidget, SLOT(slot_wireframe(bool)));
    connect(titleWidget, SIGNAL(signal_solid(bool)), glwidget, SLOT(slot_solid(bool)));
    connect(glwidget, SIGNAL(signal_mouseMoved(QVector3D)), titleWidget, SLOT(slot_sceneCoordinatesChanged(QVector3D)));
    connect(glwidget, SIGNAL(signal_selectionChanged(QList<CADitem*>)), this, SIGNAL(signal_selectionChanged(QList<CADitem*>)));
    connect(this, SIGNAL(signal_settingsChanged()), glwidget, SLOT(slot_update_settings()));
    connect(this, SIGNAL(signal_mouse3Dcoords(int,int,int,int,int,int)), glwidget, SLOT(slot_mouse3Dmoved(int,int,int,int,int,int)));
    connect(glwidget, SIGNAL(signal_highlightItem(CADitem*)), this, SIGNAL(signal_highlightItem(CADitem*)));
    connect(glwidget, SIGNAL(signal_snapFired(QVector3D,int)), this, SIGNAL(signal_snapFired(QVector3D,int)));
    connect(glwidget, SIGNAL(signal_matrix_rotation_changed(QMatrix4x4)), this, SLOT(slot_matrix_rotation_changed(QMatrix4x4)));
    connect(titleWidget, SIGNAL(signal_cuttingplane_values_changed(qreal,qreal)), glwidget, SLOT(slot_set_cuttingplane_values_changed(qreal,qreal)));

    this->slot_changeCuttingplane("Z-");

    this->resize(400, 250);
    this->setFloating(false);
    this->setMinimumSize(400, 250);
    this->setMaximumSize(4096, 3072);   // 4k
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

//    this->setMouseTracking(false);

    this->setAttribute(Qt::WA_ForceUpdatesDisabled, true);
    this->setAttribute(Qt::WA_Hover, false);
    this->setAttribute(Qt::WA_MouseTracking, false);
}

GeometryDisplay::~GeometryDisplay()
{
    //delete this->glwidget;

//    qDebug() << "GeometryDisplay destroyed";
}

void GeometryDisplay::hideButtons()
{
    titleWidget->hideButtons();
}

QStringList GeometryDisplay::getOpenGLinfo()
{
    return glwidget->getOpenGLinfo();
}

void GeometryDisplay::paintEvent(QPaintEvent *event)
{
    event->accept();
//    glwidget->updateGL();
    glwidget->update();

    return;
}

void GeometryDisplay::paintAnchor(QPainter *painter, CuttingPlane cuttingplane)
{
    QPoint bottomLeft = this->rect().bottomLeft();

    // Anchor
    // axes colors: x = red, y = green, z = blue
    QPoint anchorPoint = bottomLeft + QPoint(10, -10);

    switch (cuttingplane)
    {
        case CuttingPlane_pX:
        {
            QPen pen(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "y");
            pen.setColor(QColor(120, 120, 255));
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "z");
            break;
        }
        case CuttingPlane_nX:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::green);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "y");
            break;
        }
        case CuttingPlane_pY:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            break;
        }
        case CuttingPlane_nY:
        {
            QPen pen(Qt::red);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "x");
            pen.setColor(QColor(120, 120, 255));
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "z");
            break;
        }
        case CuttingPlane_pZ:
        {
            QPen pen(Qt::red);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "x");
            pen.setColor(Qt::green);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "y");
            break;
        }
        case CuttingPlane_nZ:
        {
            QPen pen(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "y");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            break;
        }
        case CuttingPlane_ISO_001:
        {
            QPen pen(QColor(120, 120, 255));
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(0, -50));
            painter->drawText(anchorPoint + QPoint(0, -55), "z");
            pen.setColor(Qt::red);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, 0));
            painter->drawText(anchorPoint + QPoint(55, 0), "x");
            pen.setColor(Qt::green);
            pen.setWidth(1);
            painter->setPen(pen);
            painter->drawLine(anchorPoint, anchorPoint + QPoint(50, -50));
            painter->drawText(anchorPoint + QPoint(55, -55), "-y");
            break;
        }
    }
}

void GeometryDisplay::slot_changeCuttingplane(QString directionOfView)
{
    if (directionOfView == "X-")
    {
        glwidget->set_WorldRotation(-90.0, 0.0, -90.0);
    }
    else if (directionOfView == "X+")
    {
        glwidget->set_WorldRotation(-90.0, 0.0, 90.0);
    }
    else if (directionOfView == "Y-")
    {
        glwidget->set_WorldRotation(90.0, 180.0, 0.0);
    }
    else if (directionOfView == "Y+")
    {
        glwidget->set_WorldRotation(-90.0, 0.0, 0.0);   // richtig
    }
    else if (directionOfView == "Z-")
    {
        glwidget->set_WorldRotation(0.0, 0.0, 0.0);   // richtig
    }
    else if (directionOfView == "Z+")
    {
        glwidget->set_WorldRotation(0.0, 180.0, 0.0); // richtig
    }
}


void GeometryDisplay::slot_highlightItem(CADitem *item)
{
    glwidget->slot_highlightItem(item);
}

void GeometryDisplay::slot_snapTo(QVector3D snapPos_scene, int snapMode)
{
    glwidget->slot_snapTo(snapPos_scene, snapMode);
}

void GeometryDisplay::slot_changeSelection(QList<CADitem*> selectedItems)
{
    glwidget->slot_changeSelection(selectedItems);
    titleWidget->slot_selectionCountChanged(selectedItems.count());
}

void GeometryDisplay::slot_matrix_rotation_changed(QMatrix4x4 matrix_rotation)
{
    Q_UNUSED(matrix_rotation);
    titleWidget->slot_setDirectionOfViewInvalid();
}

void GeometryDisplay::slot_float()
{
    this->setFloating(true);
}

void GeometryDisplay::slot_close()
{
    emit signal_aboutToClose(this->toggleViewAction());
    this->close();
}
