#include "cad_basic_sphere.h"

CAD_basic_sphere::CAD_basic_sphere() : CADitem(CADitem::Basic_Sphere)
{
    this->description = "Basic|Sphere";
    this->radius = 0.0;

    wizardParams.insert(QObject::tr("Center x"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center y"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Center z"), QVariant::fromValue(0.0));
    wizardParams.insert(QObject::tr("Radius"), QVariant::fromValue(1.0));
}

void CAD_basic_sphere::calculate()
{
    this->snap_basepoint = this->position;

    // tbd.: Bounding box
}

void CAD_basic_sphere::processWizardInput()
{
    position.setX(wizardParams.value(QObject::tr("Center x")).toDouble());
    position.setY(wizardParams.value(QObject::tr("Center y")).toDouble());
    position.setZ(wizardParams.value(QObject::tr("Center z")).toDouble());
    radius = wizardParams.value(QObject::tr("Radius")).toDouble();
}
