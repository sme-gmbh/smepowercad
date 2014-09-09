#ifndef CAD_HEATCOOL_BOILER_H
#define CAD_HEATCOOL_BOILER_H

#include "caditem.h"

class CAD_heatcool_boiler : public CADitem
{
public:
    CAD_heatcool_boiler();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_BOILER_H
