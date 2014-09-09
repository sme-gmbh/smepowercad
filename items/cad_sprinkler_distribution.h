#ifndef CAD_SPRINKLER_DISTRIBUTION_H
#define CAD_SPRINKLER_DISTRIBUTION_H

#include "caditem.h"

class CAD_sprinkler_distribution : public CADitem
{
public:
    CAD_sprinkler_distribution();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_DISTRIBUTION_H
