#ifndef CAD_HEATCOOL_COOLINGTOWER_H
#define CAD_HEATCOOL_COOLINGTOWER_H

#include "caditem.h"

class CAD_heatcool_coolingTower : public CADitem
{
public:
    CAD_heatcool_coolingTower();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_COOLINGTOWER_H
