#ifndef CAD_ARCH_WINDOW_H
#define CAD_ARCH_WINDOW_H

#include "caditem.h"

class CAD_arch_window : public CADitem
{
public:
    CAD_arch_window();
    virtual void calculate();
};

#endif // CAD_ARCH_WINDOW_H
