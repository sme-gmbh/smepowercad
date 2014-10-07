#include "cad_basic_arc.h"

#define PI 3.1415926535897

CAD_basic_arc::CAD_basic_arc() : CADitem(CADitem::Basic_Arc)
{
    this->description = "Basic|Arc";

    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));

    wizardParams.insert(QObject::tr("Radius"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Central Angle"), QVariant::fromValue(90.0));

    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
}

void CAD_basic_arc::calculate()
{

    this->snap_vertices.append(QVector3D(position.x()+radius, position.y(), position.z()));
    this->snap_vertices.append(QVector3D(position.x()+radius*qCos(centralAngle/180.0f*PI), position.y()+radius*qSin(centralAngle/180.0f*PI), position.z()));
    this->snap_vertices.append(QVector3D(position.x()+radius*qCos(centralAngle/360.0f*PI), position.y()+radius*qSin(centralAngle/360.0f*PI), position.z()));

}

void CAD_basic_arc::processWizardInput()
{
    this->position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    this->position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    this->position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());
    this->center = QVector3D(position.x()+radius*qCos(centralAngle/360.0f*PI), position.y()+radius*qSin(centralAngle/360.0f*PI), position.z());

    this->radius = wizardParams.value(QObject::tr("Radius")).toDouble();
    this->centralAngle = wizardParams.value(QObject::tr("Central Angle")).toDouble();

    this->angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    this->angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    this->angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();

}
