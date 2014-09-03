#ifndef SNAPENGINE_H
#define SNAPENGINE_H

#include <QThread>
#include <QPoint>

#include "itemdb.h"

class SnapEngine : public QThread
{
    Q_OBJECT
public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};

    explicit SnapEngine(ItemDB* itemDB, QObject *parent = 0);
    ~SnapEngine();
    void setUp(qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view);
    void snapProcessing(QList<Layer *> layers, QPoint mousePos);
    QList<QPointF> snap_vertex_points;


protected:
     void run();

private:
     ItemDB* itemDB;
     CuttingPlane cuttingplane;
     qreal height_of_intersection;
     qreal depth_of_view;
     qreal zoomFactor;
     QVector3D centerOfViewInScene;  // in coordsOnScene
     QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget

     QPointF mapFromScene(QVector3D scenePoint);
     qreal heightInScene(QVector3D scenePoint);


signals:

public slots:

};

#endif // SNAPENGINE_H
