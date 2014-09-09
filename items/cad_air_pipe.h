#ifndef CAD_AIR_DUCTROUND_H
#define CAD_AIR_DUCTROUND_H

#include "caditem.h"

class CAD_air_pipe : public CADitem
{
public:
    CAD_air_pipe();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTROUND_H
