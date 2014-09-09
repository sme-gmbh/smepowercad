#ifndef CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"

class CAD_air_ductValumetricFlowController : public CADitem
{
public:
    CAD_air_ductValumetricFlowController();
    virtual void calculate();
};

#endif // CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
