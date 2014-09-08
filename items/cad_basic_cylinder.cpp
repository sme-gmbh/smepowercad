#include "cad_basic_cylinder.h"

CAD_basic_cylinder::CAD_basic_cylinder() : CADitem(CADitem::Basic_Cylinder)
{
    radius = 1.0;
    center_base = QVector3D();
    height = 1.0;
    angle_x = 0.0;
    angle_y = 0.0;
    angle_z = 0.0;
}

void CAD_basic_cylinder::calculate()
{

}
