#ifndef CAD_SANITARY_PIPETEECONNECTOR_H
#define CAD_SANITARY_PIPETEECONNECTOR_H

#include "caditem.h"

class CAD_sanitary_pipeTeeConnector : public CADitem
{
public:
    CAD_sanitary_pipeTeeConnector();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPETEECONNECTOR_H
