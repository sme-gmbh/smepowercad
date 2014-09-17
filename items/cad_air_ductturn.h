#ifndef CAD_AIR_DUCTTURN_H
#define CAD_AIR_DUCTTURN_H

#include "caditem.h"

class CAD_air_ductTurn : public CADitem
{
public:
    CAD_air_ductTurn();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTTURN_H
