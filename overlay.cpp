#include <QPainter>
#include <QColor>
#include <QPen>

#include <QDebug>

#include "overlay.h"


Overlay::Overlay(QWidget *parent) :
    QWidget(parent)
{
    //this->setStyleSheet("background-color: none;");
    this->setPalette(Qt::transparent);
    this->setAttribute(Qt::WA_TransparentForMouseEvents);
    //this->setAttribute(Qt::WA_OpaquePaintEvent);    // Background widget will not be rendered
    //this->autoFillBackground();
    this->setMouseTracking(true);

    this->pickActive = false;
    this->cursorShown = true;
    this->snapMode = SnapCenter;

    qDebug() << "Overlay created";
}

Overlay::~Overlay()
{
    qDebug() << "Overlay destroyed";
}

void Overlay::paintEvent(QPaintEvent *event)
{
    if (!cursorShown)
        return;

    QPainter painter(this);
    QPen pen(Qt::white);
    pen.setWidth(1);

    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(pen);
    painter.drawLine(QPoint(0, this->mousePos.y()), QPoint(this->width() - 1, this->mousePos.y()));
    painter.drawLine(QPoint(this->mousePos.x(), 0), QPoint(this->mousePos.x(), this->height() - 1));

    if (this->pickActive)
    {
        if ((this->selectionMode() == Qt::ContainsItemShape) || (this->selectionMode() == Qt::ContainsItemBoundingRect))
        {
            painter.setPen(Qt::blue);
            painter.setBrush(QBrush(QColor(40, 40, 255, 80), Qt::SolidPattern));
        }
        else
        {
            pen.setColor(Qt::white);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);
            painter.setBrush(QBrush(QColor(40, 255, 40, 80), Qt::SolidPattern));
        }
        painter.drawRect(this->selection());
    }

    if (snapMode != SnapNo)
    {
        pen.setColor(Qt::red);
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
    }

    switch (snapMode)
    {
    case SnapBasepoint:
    {
        QRect focusRect = QRect(0, 0, 3, 3);
        focusRect.moveCenter(this->snapPos);
        painter.drawRect(focusRect);

        painter.drawLine(this->snapPos, this->snapPos + QPoint(5, -5));
        painter.drawText(this->snapPos + QPoint(7, -7), "Basepoint");
        break;
    }
    case SnapEndpoint:
    {
        QRect focusRect = QRect(0, 0, 11, 11);
        focusRect.moveCenter(this->snapPos);
        painter.drawRect(focusRect);
        painter.drawText(this->snapPos + QPoint(7, -7), "Endpoint/Vertex");
        break;
    }
    case SnapCenter:
    {
        QRect focusRect = QRect(0, 0, 3, 3);
        focusRect.moveCenter(this->snapPos);
        painter.drawRect(focusRect);

        painter.drawLine(this->snapPos - QPoint(5, -5), this->snapPos + QPoint(5, -5));
        painter.drawText(this->snapPos + QPoint(7, -7), "Center");
        break;
    }
    case SnapNo:
    {
        break;
    }
    }

    painter.end();
//    qDebug() << "Overlay: paintEvent";
    event->accept();
}


//void Overlay::mouseMoveEvent(QMouseEvent *event)
//{
//    event->accept();
//}

void Overlay::moveCursor(QPoint pos)
{
//    qDebug() << "Overlay: moveCursor " << pos.x() << " " << pos.y();
    this->mousePos = pos;
    this->cursorShown = true;
    this->repaint();
}

void Overlay::hideCursor()
{
    this->cursorShown = false;
    this->repaint();
}

void Overlay::pickStart()
{
    this->pickActive = true;
    this->pickStartPos = this->mousePos;
}

void Overlay::pickEnd()
{
    this->pickActive = false;
    this->repaint();
}

bool Overlay::isPickActive()
{
    return this->pickActive;
}

QRect Overlay::selection()
{
    // Selection rect muss immer von topleft noch bottomright gehen

    QPoint topLeft;
    topLeft.setX(qMin(this->pickStartPos.x(), this->mousePos.x()));
    topLeft.setY(qMin(this->pickStartPos.y(), this->mousePos.y()));

    QPoint bottomRight;
    bottomRight.setX(qMax(this->pickStartPos.x(), this->mousePos.x()));
    bottomRight.setY(qMax(this->pickStartPos.y(), this->mousePos.y()));

    return QRect(topLeft, bottomRight - QPoint(1, 1));
    //return QRect(this->pickStartPos.x(), this->pickStartPos.y(), this->mousePos.x() - this->pickStartPos.x(), this->mousePos.y() - this->pickStartPos.y());
}

Qt::ItemSelectionMode Overlay::selectionMode()
{
    if (this->mousePos.x() - this->pickStartPos.x() > 0)
        return Qt::ContainsItemShape;
        //return Qt::ContainsItemBoundingRect;
    else
        return Qt::IntersectsItemShape;
        //return Qt::IntersectsItemBoundingRect;
}

void Overlay::snap_enable(bool on)
{

}

void Overlay::set_snap_mode(SnapMode mode)
{
    this->snapMode = mode;
}

void Overlay::set_snapPos(QPoint snapPos)
{
    this->snapPos = snapPos;
}
