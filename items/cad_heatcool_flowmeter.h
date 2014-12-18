#ifndef CAD_HEATCOOL_FLOWMETER_H
#define CAD_HEATCOOL_FLOWMETER_H

#include "caditem.h"

class CAD_heatcool_flowmeter : public CADitem
{
public:
    CAD_heatcool_flowmeter();
    virtual ~CAD_heatcool_flowmeter();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_FLOWMETER_H
