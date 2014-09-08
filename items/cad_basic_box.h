#ifndef CAD_BASIC_BOX_H
#define CAD_BASIC_BOX_H

#include "caditem.h"

class CAD_basic_box : public CADitem
{
public:
    CAD_basic_box();
    virtual void calculate();
};

#endif // CAD_BASIC_BOX_H
