#ifndef CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H
#define CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H

#include "caditem.h"

class CAD_sprinkler_compressedAirWaterContainer : public CADitem
{
public:
    CAD_sprinkler_compressedAirWaterContainer();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H
