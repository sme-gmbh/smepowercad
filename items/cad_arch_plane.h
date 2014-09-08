#ifndef CAD_ARCH_PLANE_H
#define CAD_ARCH_PLANE_H

#include "caditem.h"

class CAD_arch_plane : public CADitem
{
public:
    CAD_arch_plane();
    virtual void calculate();
};

#endif // CAD_ARCH_PLANE_H
