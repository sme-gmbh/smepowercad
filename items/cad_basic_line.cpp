#include "cad_basic_line.h"
#include <QDebug>
#include <QPen>

CAD_basic_line::CAD_basic_line() : CADitem(CADitem::Basic_Line)
{
    this->description = "Basic|Line";

    widthByLayer = false;
    widthByBlock = false;

    this->wizardParams.insert(QObject::tr("Position x1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position y1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z1"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position x2"), QVariant::fromValue(1.0));
    this->wizardParams.insert(QObject::tr("Position y2"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Position z2"), QVariant::fromValue(0.0));
    this->wizardParams.insert(QObject::tr("Width"), QVariant::fromValue(1.0));
}

void CAD_basic_line::calculate()
{
//    this->boundingBox.p0 = this->p1;
//    this->boundingBox.a1 = QVector3D(p2.x() - p1.x(), 0.0, 0.0);
//    this->boundingBox.a2 = QVector3D(0.0, p2.y() - p1.y(), 0.0);
//    this->boundingBox.a3 = QVector3D(0.0, 0.0, p2.z() - p1.z());
    this->boundingBox.reset();
    this->boundingBox.enterVertex(this->p1);
    this->boundingBox.enterVertex(this->p2);

    this->position = this->p1;
    this->snap_vertices.append(p1);
    this->snap_vertices.append(p2);

    this->snap_center.append((p1 + p2) / 2.0);
}

void CAD_basic_line::processWizardInput()
{
    this->p1.setX(wizardParams.value(QObject::tr("Position x1")).toDouble());
    this->p1.setY(wizardParams.value(QObject::tr("Position y1")).toDouble());
    this->p1.setZ(wizardParams.value(QObject::tr("Position z1")).toDouble());


    this->p2.setX(wizardParams.value(QObject::tr("Position x2")).toDouble());
    this->p2.setY(wizardParams.value(QObject::tr("Position y2")).toDouble());
    this->p2.setZ(wizardParams.value(QObject::tr("Position z2")).toDouble());

    this->width = wizardParams.value(QObject::tr("Width")).toDouble();

    this->angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    this->angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    this->angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

}
