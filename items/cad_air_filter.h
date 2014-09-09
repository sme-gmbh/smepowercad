#ifndef CAD_AIR_FILTER_H
#define CAD_AIR_FILTER_H

#include "caditem.h"

class CAD_air_filter : public CADitem
{
public:
    CAD_air_filter();
    virtual void calculate();
};

#endif // CAD_AIR_FILTER_H
