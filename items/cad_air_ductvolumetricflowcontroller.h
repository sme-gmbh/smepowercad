#ifndef CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"

class CAD_air_ductVolumetricFlowController : public CADitem
{
public:
    CAD_air_ductVolumetricFlowController();
    virtual ~CAD_air_ductVolumetricFlowController();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
