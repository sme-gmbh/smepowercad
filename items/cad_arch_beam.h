#ifndef CAD_ARCH_BEAM_H
#define CAD_ARCH_BEAM_H

#include "caditem.h"

class CAD_arch_beam : public CADitem
{
public:
    CAD_arch_beam();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ARCH_BEAM_H
