#ifndef CAD_HEATCOOL_BUTTERFLYVALVE_H
#define CAD_HEATCOOL_BUTTERFLYVALVE_H

#include "caditem.h"

class CAD_heatcool_butterflyValve : public CADitem
{
public:
    CAD_heatcool_butterflyValve();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_BUTTERFLYVALVE_H
