#ifndef CAD_SPRINKLER_PIPEREDUCER_H
#define CAD_SPRINKLER_PIPEREDUCER_H

#include "caditem.h"

class CAD_sprinkler_pipeReducer : public CADitem
{
public:
    CAD_sprinkler_pipeReducer();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPEREDUCER_H
