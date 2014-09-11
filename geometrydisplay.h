#ifndef GEOMETRYDISPLAY_H
#define GEOMETRYDISPLAY_H

#include <QDockWidget>
#include <QCursor>
#include <QBitmap>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QMouseEvent>
#include <QRectF>
#include <QTimer>
#include <QLayout>
#include <QVector3D>
#include <QGenericMatrix>
#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QSizePolicy>
#include <QToolButton>
#include <QColor>
#include <QImage>
#include <QLabel>


#include "overlay.h"
#include "itemdb.h"
#include "snapengine.h"
#include "glwidget.h"
#include "geometrydisplaytitle.h"

class GeometryDisplay : public QDockWidget
{
    Q_OBJECT

public:
    enum CuttingPlane{CuttingPlane_pX = 0, CuttingPlane_nX = 1, CuttingPlane_pY = 2, CuttingPlane_nY = 3, CuttingPlane_pZ = 4, CuttingPlane_nZ = 5, CuttingPlane_ISO_001 = 6};

    explicit GeometryDisplay(ItemDB* itemDB, QWidget *parent = 0);
    ~GeometryDisplay();

    void setCuttingPlane(CuttingPlane cuttingplane, qreal height_of_intersection, qreal depth_of_view);
    QVector3D mapToScene(QPoint displayPoint);
    QPointF mapFromScene(QVector3D scenePoint);

    void hideButtons();
    QMap<QString, QString> getOpenGLinfo();

private:
    GeometryDisplayTitle *titleWidget;
    ItemDB* itemDB;


//    CuttingPlane cuttingplane;      // This is the direction of view
//    qreal height_of_intersection;   // This is where the intersection starts (in coordinate systam along viewing direction)
//    qreal depth_of_view;            // This is how deep viewing is (positive, in direction of view)

//    qreal zoomFactor;               // PixelsOnScreen = zoomFactor * coordsOnScene
//    qreal zoomFactor_atRenderStart;
//    qreal zoomFactor_atCurrentFrame;
//    QVector3D centerOfViewInScene;  // in coordsOnScene
//    QVector3D centerOfViewInScene_atRenderStart;  // in coordsOnScene
//    QVector3D centerOfViewInScene_atCurrentFrame;  // in coordsOnScene
//    QPoint displayCenter;           // The Center of the widget in PixelsOnScreen, related to bottomLeft of Widget
//    QPoint displayCenter_last;
//    QPoint displayCenter_atRenderStart;
//    QPoint displayCenter_atCurrentFrame;
//    float rot_x, rot_y, rot_z;

//    QRectF frameBufferSourceRect;   // Pull area of source rect; normaly 1/3 of frameBuffer (mid area), differs in case of dynamic zoom and pan

//    QList<GeometryRenderengine*> renderEngines;
//    QImage* framebufferImage;           // This is where the background content is rendered and stored
//    QImage* framebufferImage_dBuffer;   // doubleBuffer of frameBuffer

//    bool repaintNeeded;

//    SnapEngine* snapEngine;

    GLWidget *glwidget;

//    void setWindowText();
    void paintAnchor(QPainter* painter, CuttingPlane cuttingplane);


public slots:
//    void slot_redrawScene();
//    void slot_renderEngineFinished(QImage* frameBufferImage);
//    void slot_setHeightOfIntersection(qreal height);
    void slot_changeCuttingplane(QString directionOfView);

private slots:
//    void slot_timer_50ms_fired();
    void slot_float();
    void slot_close();


protected:
//    virtual void wheelEvent(QWheelEvent *event);
//    virtual void mouseMoveEvent(QMouseEvent *event);

//    virtual void enterEvent(QEvent *event);
//    virtual void leaveEvent(QEvent *event);
//    virtual void mousePressEvent(QMouseEvent *event);
//    virtual void mouseReleaseEvent(QMouseEvent *event);

//    virtual void keyPressEvent(QKeyEvent *event);

//    virtual void resizeEvent(QResizeEvent *event);
    virtual void paintEvent(QPaintEvent *event);

signals:
    void signal_sceneCoordinateChanged(QVector3D sceneCoord);
    void signal_aboutToClose(QAction *action);
    void signal_repaintNeeded();
    void signal_mouse3Dcoords(int x, int y, int z, int a, int b, int c);

};

#endif // GEOMETRYDISPLAY_H
