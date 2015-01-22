#ifndef CAD_AIR_DUCTFIRERESISTANT_H
#define CAD_AIR_DUCTFIRERESISTANT_H

#include "caditem.h"

class CAD_air_ductFireResistant : public CADitem
{
public:
    CAD_air_ductFireResistant();
    virtual ~CAD_air_ductFireResistant();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTFIRERESISTANT_H
