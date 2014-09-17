#ifndef CAD_AIR_DUCTFIREDAMPER_H
#define CAD_AIR_DUCTFIREDAMPER_H

#include "caditem.h"

class CAD_air_ductFireDamper : public CADitem
{
public:
    CAD_air_ductFireDamper();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTFIREDAMPER_H
