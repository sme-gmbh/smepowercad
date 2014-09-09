#ifndef CAD_HEATCOOL_PUMP_H
#define CAD_HEATCOOL_PUMP_H

#include "caditem.h"

class CAD_heatcool_pump : public CADitem
{
public:
    CAD_heatcool_pump();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_PUMP_H
