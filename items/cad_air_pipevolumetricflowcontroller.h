#ifndef CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"

class CAD_air_pipeVolumetricFlowController : public CADitem
{
public:
    CAD_air_pipeVolumetricFlowController();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
