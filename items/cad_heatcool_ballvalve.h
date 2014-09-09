#ifndef CAD_HEATCOOL_BALLVALVE_H
#define CAD_HEATCOOL_BALLVALVE_H

#include "caditem.h"

class CAD_heatcool_ballValve : public CADitem
{
public:
    CAD_heatcool_ballValve();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_BALLVALVE_H
