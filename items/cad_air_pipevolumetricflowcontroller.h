#ifndef CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"

class CAD_air_pipeVolumetricFlowController : public CADitem
{
public:
    CAD_air_pipeVolumetricFlowController();
    virtual ~CAD_air_pipeVolumetricFlowController();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPEVOLUMETRICFLOWCONTROLLER_H
