#ifndef CAD_HEATCOOL_PIPEENDCAP_H
#define CAD_HEATCOOL_PIPEENDCAP_H

#include "caditem.h"

class CAD_heatcool_pipeEndCap : public CADitem
{
public:
    CAD_heatcool_pipeEndCap();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_PIPEENDCAP_H
