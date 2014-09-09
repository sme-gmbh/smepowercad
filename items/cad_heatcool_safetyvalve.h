#ifndef CAD_HEATCOOL_SAFETYVALVE_H
#define CAD_HEATCOOL_SAFETYVALVE_H

#include "caditem.h"

class CAD_heatcool_safetyValve : public CADitem
{
public:
    CAD_heatcool_safetyValve();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_SAFETYVALVE_H
