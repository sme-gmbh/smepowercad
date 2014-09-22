#ifndef CAD_SPRINKLER_PIPEENDCAP_H
#define CAD_SPRINKLER_PIPEENDCAP_H

#include "caditem.h"

class CAD_sprinkler_pipeEndCap : public CADitem
{
public:
    CAD_sprinkler_pipeEndCap();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPEENDCAP_H
