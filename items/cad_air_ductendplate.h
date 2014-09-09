#ifndef CAD_AIR_DUCTENDPLATE_H
#define CAD_AIR_DUCTENDPLATE_H

#include "caditem.h"

class CAD_air_ductEndPlate : public CADitem
{
public:
    CAD_air_ductEndPlate();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTENDPLATE_H
