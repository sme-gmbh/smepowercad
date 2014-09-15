#include "cad_basic_cylinder.h"

CAD_basic_cylinder::CAD_basic_cylinder() : CADitem(CADitem::Basic_Cylinder)
{
    this->description = "Basic|Cylinder";
    radius = 1.0;
    center_base = QVector3D();
    height = 1.0;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;

    this->wizardParams.insert("Position x", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position y", QVariant::fromValue(0.0));
    this->wizardParams.insert("Position z", QVariant::fromValue(0.0));
    this->wizardParams.insert("Height", QVariant::fromValue(0.0));
    this->wizardParams.insert("Angle x", QVariant::fromValue(0.0));
    this->wizardParams.insert("Angle y", QVariant::fromValue(0.0));
    this->wizardParams.insert("Angle z", QVariant::fromValue(0.0));
    this->wizardParams.insert("Name", QVariant::fromValue(QString("Default Name")));
}

void CAD_basic_cylinder::calculate()
{
    this->snap_basepoint = this->center_base;
}

void CAD_basic_cylinder::processWizardInput()
{

}
