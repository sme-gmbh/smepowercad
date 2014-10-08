#ifndef CAD_SANITARY_EMERGENCYEYESHOWER_H
#define CAD_SANITARY_EMERGENCYEYESHOWER_H

#include "caditem.h"

class CAD_sanitary_emergencyEyeShower : public CADitem
{
public:
    CAD_sanitary_emergencyEyeShower();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_EMERGENCYEYESHOWER_H
