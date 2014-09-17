#ifndef CAD_BASIC_ARC_H
#define CAD_BASIC_ARC_H

#include "caditem.h"

class CAD_basic_arc : public CADitem
{
public:
    CAD_basic_arc();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_BASIC_ARC_H
