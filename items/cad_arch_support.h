#ifndef CAD_ARCH_SUPPORT_H
#define CAD_ARCH_SUPPORT_H

#include "caditem.h"

class CAD_arch_support : public CADitem
{
public:
    CAD_arch_support();
    virtual void calculate();
};

#endif // CAD_ARCH_SUPPORT_H
