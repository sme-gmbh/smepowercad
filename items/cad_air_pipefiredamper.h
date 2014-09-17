#ifndef CAD_AIR_PIPEFIREDAMPER_H
#define CAD_AIR_PIPEFIREDAMPER_H

#include "caditem.h"

class CAD_air_pipeFireDamper : public CADitem
{
public:
    CAD_air_pipeFireDamper();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPEFIREDAMPER_H
