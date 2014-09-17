#ifndef CAD_AIR_DUCTTRANSITION_H
#define CAD_AIR_DUCTTRANSITION_H

#include "caditem.h"

class CAD_air_ductTransition : public CADitem
{
public:
    CAD_air_ductTransition();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTTRANSITION_H
