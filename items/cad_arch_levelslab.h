#ifndef CAD_ARCH_LEVELSLAB_H
#define CAD_ARCH_LEVELSLAB_H

#include "caditem.h"

class CAD_arch_levelSlab : public CADitem
{
public:
    CAD_arch_levelSlab();
    virtual void calculate();
};

#endif // CAD_ARCH_LEVELSLAB_H
