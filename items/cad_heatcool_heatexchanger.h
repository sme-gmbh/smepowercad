#ifndef CAD_HEATCOOL_HEATEXCHANGER_H
#define CAD_HEATCOOL_HEATEXCHANGER_H

#include "caditem.h"

class CAD_heatcool_heatExchanger : public CADitem
{
public:
    CAD_heatcool_heatExchanger();
    virtual ~CAD_heatcool_heatExchanger();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_HEATEXCHANGER_H
