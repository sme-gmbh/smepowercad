#ifndef CAD_HEATCOOL_ADJUSTVALVE_H
#define CAD_HEATCOOL_ADJUSTVALVE_H

#include "caditem.h"

class CAD_heatcool_adjustvalve : public CADitem
{
public:
    CAD_heatcool_adjustvalve();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_ADJUSTVALVE_H
