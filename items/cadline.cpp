#include "../items/cadline.h"
#include <QDebug>
#include <QPen>

CADline::CADline() : CADitem(CADitem::Line)
{
    widthByLayer = false;
    widthByBlock = false;
}

void CADline::calculate()
{
    this->boundingBox.p0 = this->p1;
    this->boundingBox.a1 = QVector3D(p2.x() - p1.x(), 0.0, 0.0);
    this->boundingBox.a2 = QVector3D(0.0, p2.y() - p1.y(), 0.0);
    this->boundingBox.a3 = QVector3D(0.0, 0.0, p2.z() - p1.z());
    this->position = this->p1;
//    this->snap_center = ((M3dVector)(M3dVector(p1) *0.5 + M3dVector(p2) * 0,5)).toM3dPoint();
    this->snap_vertices.append(p1);
    this->snap_vertices.append(p2);
}
