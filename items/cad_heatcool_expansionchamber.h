#ifndef CAD_HEATCOOL_EXPANSIONCHAMBER_H
#define CAD_HEATCOOL_EXPANSIONCHAMBER_H

#include "caditem.h"

class CAD_heatcool_expansionChamber : public CADitem
{
public:
    CAD_heatcool_expansionChamber();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_EXPANSIONCHAMBER_H
