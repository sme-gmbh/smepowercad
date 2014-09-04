#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QDebug>
#include <QEvent>
#include <QPaintEvent>

#include "itemdb.h"


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

private:
    ItemDB* itemDB;
    CuttingPlane cuttingplane;
    qreal height_of_intersection;
    qreal depth_of_view;
    QList<Layer*> layers;
    QPoint translationOffset;
    qreal zoomFactor;
    QVector3D centerOfViewInScene;  // in coordsOnScene
    QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget
    float rot_x, rot_y, rot_z;

    GLuint tile_list;

    // Overlay
    QPoint mousePos;
    QPoint pickStartPos;
    bool pickActive;
    bool cursorShown;
    SnapMode snapMode;
    QPoint snapPos;


    // OpenGL
    void saveGLState();
    void restoreGLState();

    void paintContent(QList<Layer*> layers);
    void paintLine(Layer* layer, CADline* item);
    void paintPolyLine(Layer *layer, CADpolyline *item);
    void paintFace(Layer *layer, CAD3Dface *item);


protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // GLWIDGET_H
