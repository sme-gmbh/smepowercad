#ifndef CAD_HEATCOOL_WATERHEATER_H
#define CAD_HEATCOOL_WATERHEATER_H

#include "caditem.h"

class CAD_heatcool_waterHeater : public CADitem
{
public:
    CAD_heatcool_waterHeater();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_WATERHEATER_H
