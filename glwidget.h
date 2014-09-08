#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>
#include <QEvent>
#include <QPaintEvent>
#include <QMatrix4x4>

#include <qmath.h>
#include <qgl.h>
#include <GL/glu.h>

#include "itemdb.h"
#include "snapengine.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT
public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};


    explicit GLWidget(QWidget *parent, ItemDB *itemDB);
    ~GLWidget();

    void setup(QPoint translationOffset, qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view, qreal rot_x, qreal rot_y, qreal rot_z);

    // Overlay
    void moveCursor(QPoint pos);
    void hideCursor();
    void pickStart();
    void pickEnd();
    bool isPickActive();
    QRect selection();
    Qt::ItemSelectionMode selectionMode();

    enum SnapMode{SnapNo, SnapEndpoint, SnapCenter, SnapBasepoint};
    void snap_enable(bool on);
    void set_snap_mode(SnapMode mode);
    void set_snapPos(QPoint snapPos);
    void set_WorldRotation(float rot_x, float rot_y, float rot_z);

private:
    ItemDB* itemDB;
    QPoint mousePosOld;
    CuttingPlane cuttingplane;
    qreal height_of_intersection;
    qreal depth_of_view;
//    QList<Layer*> layers;
    QPoint translationOffset;
    qreal zoomFactor;
    QVector3D centerOfViewInScene;  // in coordsOnScene
    QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget
    QVector3D cameraPosition;
    float rot_x, rot_y, rot_z;

    bool render_solid;
    bool render_outline;

    GLuint tile_list;

    QMap<GLuint, CADitem*> glNameMap;

    // Overlay
    QPoint mousePos;
    QPoint pickStartPos;
    bool pickActive;
    bool cursorShown;
    SnapMode snapMode;
    QPoint snapPos;

    SnapEngine* snapEngine;


    // OpenGL
    void saveGLState();
    void restoreGLState();

    void paintContent(QList<Layer*> layers);
    void paintLine(Layer* layer, CAD_basic_line* item);
    void paintPolyLine(Layer *layer, CAD_basic_polyline *item);
    void paintFace(Layer *layer, CAD_basic_3Dface *item);
    void paintBasicCircle(Layer *layer, CAD_basic_circle *item);
    void paintBasicBox(Layer *layer, CAD_basic_box *item);
    CADitem *itemAtPosition(QPoint pos);
    CADitem *itemAtPosition_processLayers(QList<Layer*> layers, GLuint glName);
    CADitem *itemAtPosition_processItems(QList<CADitem*> items, GLuint glName);
    void highlightItemAtPosition(QPoint pos);
    void highlightClear();
    void highlightClear_processLayers(QList<Layer*> layers);
    void highlightClear_processItems(QList<CADitem*> items);

protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);


signals:

public slots:
    void slot_repaint();
    void slot_wireframe(bool on);
    void slot_solid(bool on);
};

#endif // GLWIDGET_H
