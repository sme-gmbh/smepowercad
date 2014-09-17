#ifndef CAD_AIR_DUCTTEECONNECTOR_H
#define CAD_AIR_DUCTTEECONNECTOR_H

#include "caditem.h"

class CAD_air_ductTeeConnector : public CADitem
{
public:
    CAD_air_ductTeeConnector();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTTEECONNECTOR_H
