#ifndef CAD_AIR_PIPESILENCER_H
#define CAD_AIR_PIPESILENCER_H

#include "caditem.h"

class CAD_air_pipeSilencer : public CADitem
{
public:
    CAD_air_pipeSilencer();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPESILENCER_H
