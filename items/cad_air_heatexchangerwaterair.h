#ifndef CAD_AIR_HEATEXCHANGERWATERAIR_H
#define CAD_AIR_HEATEXCHANGERWATERAIR_H

#include "caditem.h"

class CAD_air_heatExchangerWaterAir : public CADitem
{
public:
    CAD_air_heatExchangerWaterAir();
    virtual void calculate();
};

#endif // CAD_AIR_HEATEXCHANGERWATERAIR_H
