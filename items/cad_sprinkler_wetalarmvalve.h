#ifndef CAD_SPRINKLER_WETALARMVALVE_H
#define CAD_SPRINKLER_WETALARMVALVE_H

#include "caditem.h"

class CAD_sprinkler_wetAlarmValve : public CADitem
{
public:
    CAD_sprinkler_wetAlarmValve();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_WETALARMVALVE_H
