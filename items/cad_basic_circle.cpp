#include "cad_basic_circle.h"

CAD_basic_circle::CAD_basic_circle() : CADitem(CADitem::Basic_Circle)
{
    this->description = "Basic|Circle";
    this->radius = 1;
    this->center = QVector3D(0.0, 0.0, 0.0);
    this->width = 0.0;
    this->widthByLayer = true;
    this->widthByBlock = false;
}

void CAD_basic_circle::calculate()
{
    this->snap_basepoint = this->center;
}

void CAD_basic_circle::processWizardInput()
{

}
