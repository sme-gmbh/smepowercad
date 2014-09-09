#ifndef CAD_AIR_EQUIPMENTFRAME_H
#define CAD_AIR_EQUIPMENTFRAME_H

#include "caditem.h"

class CAD_air_equipmentFrame : public CADitem
{
public:
    CAD_air_equipmentFrame();
    virtual void calculate();
};

#endif // CAD_AIR_EQUIPMENTFRAME_H
