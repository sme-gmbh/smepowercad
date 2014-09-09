#ifndef CAD_AIR_THROTTLEVALVE_H
#define CAD_AIR_THROTTLEVALVE_H

#include "caditem.h"

class CAD_air_throttleValve : public CADitem
{
public:
    CAD_air_throttleValve();
    virtual void calculate();
};

#endif // CAD_AIR_THROTTLEVALVE_H
