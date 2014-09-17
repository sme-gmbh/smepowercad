#include "cad_basic_line.h"
#include <QDebug>
#include <QPen>

CAD_basic_line::CAD_basic_line() : CADitem(CADitem::Basic_Line)
{
    this->description = "Basic|Line";

    widthByLayer = false;
    widthByBlock = false;

    this->wizardParams.insert(QObject::tr("Position x0"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position y0"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z0"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position x1"), QVariant::fromValue(1.0));
    this->wizardParams.insert(QObject::tr("Position y2"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z3"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Width"), QVariant::fromValue(1.0));
}

void CAD_basic_line::calculate()
{
    this->boundingBox.p0 = this->p1;
    this->boundingBox.a1 = QVector3D(p2.x() - p1.x(), 0.0, 0.0);
    this->boundingBox.a2 = QVector3D(0.0, p2.y() - p1.y(), 0.0);
    this->boundingBox.a3 = QVector3D(0.0, 0.0, p2.z() - p1.z());
    this->position = this->p1;
//    this->snap_center = ((M3dVector)(M3dVector(p1) *0.5 + M3dVector(p2) * 0,5)).toM3dPoint();
    this->snap_vertices.append(p1);
    this->snap_vertices.append(p2);

    this->snap_center.append((p1 + p2) / 2.0);
}

void CAD_basic_line::processWizardInput()
{

}
