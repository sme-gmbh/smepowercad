#include "cad_basic_circle.h"

CAD_basic_circle::CAD_basic_circle() : CADitem(CADitem::Basic_Circle)
{
    this->description = "Basic|Circle";
    this->radius = 1;
    this->center = QVector3D(0.0, 0.0, 0.0);
    this->width = 0.0;
    this->widthByLayer = true;
    this->widthByBlock = false;

    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Radius"), QVariant::fromValue(1.0));
    wizardParams.insert(QObject::tr("Angle x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Angle z"), QVariant::fromValue(0.0));
}

void CAD_basic_circle::calculate()
{
    this->snap_basepoint = this->center;
}

void CAD_basic_circle::processWizardInput()
{
    center.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    center.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    center.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());
    radius = wizardParams.value(QObject::tr("Radius")).toDouble();
    angle_x = wizardParams.value(QObject::tr("Angle x")).toDouble();
    angle_y = wizardParams.value(QObject::tr("Angle y")).toDouble();
    angle_z = wizardParams.value(QObject::tr("Angle z")).toDouble();
}
