#ifndef CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
#define CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H

#include "caditem.h"
#include "cad_air_duct.h"
#include "cad_basic_box.h"

class CAD_air_ductVolumetricFlowController : public CADitem
{
public:
    CAD_air_ductVolumetricFlowController();
    virtual ~CAD_air_ductVolumetricFlowController();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();

    qreal ff, fe, s;
    qreal a, b, l;
    CAD_air_duct *duct;
    CAD_basic_box *function, *flap;
};

#endif // CAD_AIR_DUCTVOLUMETRICFLOWCONTROLLER_H
