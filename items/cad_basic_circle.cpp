#include "cad_basic_circle.h"

CAD_basic_circle::CAD_basic_circle() : CADitem(CADitem::Basic_Circle)
{
    this->radius = 1;
    this->center = QVector3D(0.0, 0.0, 0.0);
    this->width = 0.0;
    this->widthByLayer = true;
    this->widthByBlock = false;
}

void CAD_basic_circle::calculate()
{

}
