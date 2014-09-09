#ifndef CAD_AIR_HEATEXCHANGERAIRAIR_H
#define CAD_AIR_HEATEXCHANGERAIRAIR_H

#include "caditem.h"

class CAD_air_heatExchangerAirAir : public CADitem
{
public:
    CAD_air_heatExchangerAirAir();
    virtual void calculate();
};

#endif // CAD_AIR_HEATEXCHANGERAIRAIR_H
