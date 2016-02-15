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

#ifndef GEOMETRYDISPLAY_H
#define GEOMETRYDISPLAY_H

#include <QApplication>
#include <QBitmap>
#include <QColor>
#include <QCursor>
#include <QDockWidget>
#include <QGenericMatrix>
#include <QImage>
#include <QKeyEvent>
#include <QLabel>
#include <QLayout>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QOpenGLContext>
#include <QPainter>
#include <QRectF>
#include <QSizePolicy>
#include <QTimer>
#include <QToolButton>
#include <QVector3D>
#include <QWheelEvent>
#include <qmath.h>

#include "itemdb.h"
#include "glwidget.h"
#include "geometrydisplaytitle.h"
#include "itemgripmodifier.h"

class GeometryDisplay : public QDockWidget
{
    Q_OBJECT

public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};

    explicit GeometryDisplay(ItemDB *itemDB, ItemWizard *itemWizard, ItemGripModifier* itemGripModifier, QWidget *parent = 0);
    ~GeometryDisplay();

    void setCuttingPlane(CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view);
    QVector3D mapToScene(QPoint displayPoint);
    QPointF mapFromScene(QVector3D scenePoint);

    void hideButtons();
    QStringList getOpenGLinfo();
    QOpenGLContext *getContext();
    GLWidget* getWidget();

    void clearTitleWidget();

private:
    GeometryDisplayTitle *titleWidget;
    ItemDB* itemDB;
    ItemWizard *itemWizard;
    ItemGripModifier* itemGripModifier;

    GLWidget *glwidget;

    bool m_showTitleOnlyOnHover;
    bool m_cursorInWindow;

public slots:
    void slot_changeCuttingplane(QString directionOfView);

    void slot_highlightItem(CADitem* item);
    void slot_snapTo(QVector3D snapPos_scene, int snapMode);
    void slot_changeSelection(QList<CADitem*> selectedItems);
    void slot_matrix_rotation_changed(QMatrix4x4 matrix_rotation);


private slots:
    void slot_float();
    void slot_close();


protected:
    virtual void paintEvent(QPaintEvent *event);

signals:
    void signal_sceneCoordinateChanged(QVector3D sceneCoord);
    void signal_aboutToClose(QAction *action);
    void signal_repaintNeeded();
    void signal_mouse3Dcoords(int x, int y, int z, int a, int b, int c);
    void signal_settingsChanged();
    void signal_highlightItem(CADitem* item);
    void signal_snapFired(QVector3D snapPos_scene, int snapMode);
    void signal_selectionChanged(QList<CADitem*>);
    void signal_itemDeleted(CADitem* item);
};

#endif // GEOMETRYDISPLAY_H
