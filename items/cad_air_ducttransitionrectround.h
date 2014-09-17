#ifndef CAD_AIR_DUCTTRANSITIONRECTROUND_H
#define CAD_AIR_DUCTTRANSITIONRECTROUND_H

#include "caditem.h"

class CAD_air_ductTransitionRectRound : public CADitem
{
public:
    CAD_air_ductTransitionRectRound();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTTRANSITIONRECTROUND_H
