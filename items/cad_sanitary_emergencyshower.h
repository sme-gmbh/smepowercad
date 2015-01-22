#ifndef CAD_SANITARY_EMERGENCYSHOWER_H
#define CAD_SANITARY_EMERGENCYSHOWER_H

#include "caditem.h"

class CAD_sanitary_emergencyShower : public CADitem
{
public:
    CAD_sanitary_emergencyShower();
    virtual ~CAD_sanitary_emergencyShower();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_EMERGENCYSHOWER_H
