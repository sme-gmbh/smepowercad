#ifndef GEOMETRYRENDERENGINE_H
#define GEOMETRYRENDERENGINE_H

#include <QThread>
#include <QPainter>
#include <QPaintEngine>
#include <QImage>
#include <QRgb>
#include <QList>
#include <QtOpenGL/QGLWidget>
#include <qmath.h>

#include "itemdb.h"

class GeometryRenderengine : public QThread
{
    Q_OBJECT
public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};

    explicit GeometryRenderengine(QObject *parent, ItemDB* itemDB);
    ~GeometryRenderengine();
    void setFrameBufferImageSize(QSize size);
    void addLayer(Layer* layer);
    void renderLayers(QPoint translationOffset, qreal zoomFactor, QVector3D centerOfViewInScene, QPoint displayCenter, CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view);

protected:
     void run();

private:
     ItemDB* itemDB;
     CuttingPlane cuttingplane;
     qreal height_of_intersection;
     qreal depth_of_view;
     QImage* framebufferImage;
     QList<qreal> depthbufferImage;
     QSize frameBufferSizeNew;
     QSize frameBufferSize;
     int frameBufferWidth;
     int frameBufferHeight;
     QList<Layer*> layers;
     QPoint translationOffset;
     qreal zoomFactor;
     QVector3D centerOfViewInScene;  // in coordsOnScene
     QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget

     QPointF mapFromScene(QVector3D scenePoint);
     qreal heightInScene(QVector3D scenePoint);

     void paintContent(QPainter *painter, QList<Layer*> layers);
     inline void paintPixel(int &x, int &y, qreal &depth, QRgb &color);
     void paintLine(QPainter* painter, Layer* layer, CAD_basic_line* item);
     void paintLine2(QPainter* painter, Layer* layer, CAD_basic_line* item);
     void paintPolyLine(QPainter *painter, Layer *layer, CAD_basic_polyline *item);
     void paintFace(QPainter *painter, Layer *layer, CAD_basic_3Dface *item);
     void renderTriangle(int x0, int x1, int x2, int y0, int y1, int y2, qreal d0, qreal d1, qreal d2, QRgb &colorPen, QRgb &colorBrush);

signals:
     void signal_renderingDone(QImage* framebufferImage);

public slots:

private slots:
     void slot_renderingDone();

};

#endif // GEOMETRYRENDERENGINE_H
