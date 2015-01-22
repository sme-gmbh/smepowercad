#ifndef CAD_HEATCOOL_ADJUSTVALVE_H
#define CAD_HEATCOOL_ADJUSTVALVE_H

#include "caditem.h"

class CAD_heatcool_adjustvalve : public CADitem
{
public:
    CAD_heatcool_adjustvalve();
    virtual ~CAD_heatcool_adjustvalve();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_ADJUSTVALVE_H
