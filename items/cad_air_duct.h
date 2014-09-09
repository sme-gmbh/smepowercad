#ifndef CAD_AIR_DUCTRECT_H
#define CAD_AIR_DUCTRECT_H

#include "caditem.h"

class CAD_air_duct : public CADitem
{
public:
    CAD_air_duct();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTRECT_H
