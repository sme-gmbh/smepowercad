#ifndef CAD_BASIC_CYLINDER_H
#define CAD_BASIC_CYLINDER_H

#include "caditem.h"

class CAD_basic_cylinder : public CADitem
{
public:
    CAD_basic_cylinder();
    virtual void calculate();
};

#endif // CADCYLINDER_H
