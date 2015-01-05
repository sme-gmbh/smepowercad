#ifndef CAD_HEATCOOL_COOLINGTOWER_H
#define CAD_HEATCOOL_COOLINGTOWER_H

#include "caditem.h"

class CAD_heatcool_coolingTower : public CADitem
{
public:
    CAD_heatcool_coolingTower();
    virtual ~CAD_heatcool_coolingTower();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_COOLINGTOWER_H
