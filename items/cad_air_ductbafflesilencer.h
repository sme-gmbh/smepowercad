#ifndef CAD_AIR_DUCTBAFFLESILENCER_H
#define CAD_AIR_DUCTBAFFLESILENCER_H

#include "caditem.h"

class CAD_air_ductBaffleSilencer : public CADitem
{
public:
    CAD_air_ductBaffleSilencer();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTBAFFLESILENCER_H
