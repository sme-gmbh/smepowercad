#ifndef CAD_SANITARY_LIFTINGUNIT_H
#define CAD_SANITARY_LIFTINGUNIT_H

#include "caditem.h"

class CAD_sanitary_liftingUnit : public CADitem
{
public:
    CAD_sanitary_liftingUnit();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_LIFTINGUNIT_H
