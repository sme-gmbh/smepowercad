#ifndef CAD_SPRINKLER_VALVE_H
#define CAD_SPRINKLER_VALVE_H

#include "caditem.h"

class CAD_sprinkler_valve : public CADitem
{
public:
    CAD_sprinkler_valve();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_VALVE_H
