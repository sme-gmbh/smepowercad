#ifndef CAD_AIR_EMPTYCABINET_H
#define CAD_AIR_EMPTYCABINET_H

#include "caditem.h"

class CAD_air_emptyCabinet : public CADitem
{
public:
    CAD_air_emptyCabinet();
    virtual void calculate();
};

#endif // CAD_AIR_EMPTYCABINET_H
