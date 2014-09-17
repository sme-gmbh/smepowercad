#ifndef CAD_SPRINKLER_PUMP_H
#define CAD_SPRINKLER_PUMP_H

#include "caditem.h"

class CAD_sprinkler_pump : public CADitem
{
public:
    CAD_sprinkler_pump();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PUMP_H
