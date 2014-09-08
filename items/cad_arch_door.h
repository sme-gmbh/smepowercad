#ifndef CAD_ARCH_DOOR_H
#define CAD_ARCH_DOOR_H

#include "caditem.h"

class CAD_arch_door : public CADitem
{
public:
    CAD_arch_door();
    virtual void calculate();
};

#endif // CAD_ARCH_DOOR_H
