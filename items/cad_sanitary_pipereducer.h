#ifndef CAD_SANITARY_PIPEREDUCER_H
#define CAD_SANITARY_PIPEREDUCER_H

#include "caditem.h"

class CAD_sanitary_pipeReducer : public CADitem
{
public:
    CAD_sanitary_pipeReducer();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPEREDUCER_H
