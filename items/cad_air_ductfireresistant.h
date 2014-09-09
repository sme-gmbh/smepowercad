#ifndef CAD_AIR_DUCTFIRERESISTANT_H
#define CAD_AIR_DUCTFIRERESISTANT_H

#include "caditem.h"

class CAD_air_ductFireResistant : public CADitem
{
public:
    CAD_air_ductFireResistant();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTFIRERESISTANT_H
