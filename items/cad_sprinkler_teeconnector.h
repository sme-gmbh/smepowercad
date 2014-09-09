#ifndef CAD_SPRINKLER_TEECONNECTOR_H
#define CAD_SPRINKLER_TEECONNECTOR_H

#include "caditem.h"

class CAD_sprinkler_teeConnector : public CADitem
{
public:
    CAD_sprinkler_teeConnector();
    virtual void calculate();
};

#endif // CAD_SPRINKLER_TEECONNECTOR_H
