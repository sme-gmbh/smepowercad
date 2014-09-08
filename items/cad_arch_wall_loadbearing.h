#ifndef CAD_ARCH_WALL_LOADBEARING_H
#define CAD_ARCH_WALL_LOADBEARING_H

#include "caditem.h"

class CAD_arch_wall_loadBearing : public CADitem
{
public:
    CAD_arch_wall_loadBearing();
    virtual void calculate();
};

#endif // CAD_ARCH_WALL_LOADBEARING_H
