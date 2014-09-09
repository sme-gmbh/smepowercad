#ifndef CAD_AIR_PIPEENDCAP_H
#define CAD_AIR_PIPEENDCAP_H

#include "caditem.h"

class CAD_air_pipeEndCap : public CADitem
{
public:
    CAD_air_pipeEndCap();
    virtual void calculate();
};

#endif // CAD_AIR_PIPEENDCAP_H
