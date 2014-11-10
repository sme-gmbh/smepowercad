#ifndef CAD_HEATCOOL_BUTTERFLYVALVE_H
#define CAD_HEATCOOL_BUTTERFLYVALVE_H

#include "caditem.h"

class CAD_heatcool_butterflyValve : public CADitem
{
public:
    CAD_heatcool_butterflyValve();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_BUTTERFLYVALVE_H
