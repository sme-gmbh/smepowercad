#ifndef CAD_HEATCOOL_FLANGE_H
#define CAD_HEATCOOL_FLANGE_H

#include "caditem.h"

class CAD_heatcool_flange : public CADitem
{
public:
    CAD_heatcool_flange();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_FLANGE_H
