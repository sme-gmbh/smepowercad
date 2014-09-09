#ifndef CAD_AIR_FAN_H
#define CAD_AIR_FAN_H

#include "caditem.h"

class CAD_air_fan : public CADitem
{
public:
    CAD_air_fan();
    virtual void calculate();
};

#endif // CAD_AIR_FAN_H
