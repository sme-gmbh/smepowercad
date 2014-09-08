#ifndef CAD_ARCH_WALL_NONLOADBEARING_H
#define CAD_ARCH_WALL_NONLOADBEARING_H

#include "caditem.h"

class CAD_arch_wall_nonLoadBearing : public CADitem
{
public:
    CAD_arch_wall_nonLoadBearing();
    virtual void calculate();
};

#endif // CAD_ARCH_WALL_NONLOADBEARING_H
