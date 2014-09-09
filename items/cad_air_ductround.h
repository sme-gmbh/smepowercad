#ifndef CAD_AIR_DUCTROUND_H
#define CAD_AIR_DUCTROUND_H

#include "caditem.h"

class CAD_air_DuctRound : public CADitem
{
public:
    CAD_air_DuctRound();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTROUND_H
