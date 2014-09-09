#ifndef CAD_SPRINKLER_ZONECHECK_H
#define CAD_SPRINKLER_ZONECHECK_H

#include "caditem.h"

class CAD_sprinkler_zoneCheck : public CADitem
{
public:
    CAD_sprinkler_zoneCheck();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_ZONECHECK_H
