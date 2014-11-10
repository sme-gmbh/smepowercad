#ifndef CAD_HEATCOOL_CONTROLVALVE_H
#define CAD_HEATCOOL_CONTROLVALVE_H

#include "caditem.h"

class CAD_heatcool_controlvalve : public CADitem
{
public:
    CAD_heatcool_controlvalve();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_CONTROLVALVE_H
