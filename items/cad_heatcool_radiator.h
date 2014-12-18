#ifndef CAD_HEATCOOL_RADIATOR_H
#define CAD_HEATCOOL_RADIATOR_H

#include "caditem.h"

class CAD_heatcool_radiator : public CADitem
{
public:
    CAD_heatcool_radiator();
    virtual ~CAD_heatcool_radiator();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_RADIATOR_H
