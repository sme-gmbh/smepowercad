#include "geometrydisplay.h"

#include <QDebug>

#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <qmath.h>

GeometryDisplay::GeometryDisplay(ItemDB *itemDB, QWidget *parent) :
    QDockWidget(parent)
{
    qDebug() << "GeometryDisplay created";
    this->itemDB = itemDB;

    titleWidget = new GeometryDisplayTitle();
    connect(this, SIGNAL(signal_sceneCoordinateChanged(QVector3D)), titleWidget, SLOT(slot_sceneCoordinatesChanged(QVector3D)));
    connect(titleWidget, SIGNAL(signal_float()), this, SLOT(slot_float()));
    connect(titleWidget, SIGNAL(signal_close()), this, SLOT(slot_close()));
    this->setTitleBarWidget(titleWidget);

    this->setWindowTitle("Zeichnung x");


    glwidget = new GLWidget(this, itemDB);
    this->setWidget(glwidget);
    connect(this, SIGNAL(signal_repaintNeeded()), glwidget, SLOT(slot_repaint()));

    this->resize(400, 250);
    this->setFloating(false);
    this->setMinimumSize(400, 250);
    this->setMaximumSize(4096, 3072);   // 4k ultra hd
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    this->setMouseTracking(true);
}

GeometryDisplay::~GeometryDisplay()
{
    delete this->glwidget;

    qDebug() << "GeometryDisplay destroyed";
}

void GeometryDisplay::hideButtons()
{
    titleWidget->hideButtons();
}

void GeometryDisplay::paintEvent(QPaintEvent *event)
{
    event->accept();
    glwidget->updateGL();

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

void GeometryDisplay::slot_float()
{
    this->setFloating(true);
}

void GeometryDisplay::slot_close()
{
    emit signal_aboutToClose(this->toggleViewAction());
    this->close();
}
