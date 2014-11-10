#ifndef CAD_AIR_DUCTFIRERESISTANT_H
#define CAD_AIR_DUCTFIRERESISTANT_H

#include "caditem.h"

class CAD_air_ductFireResistant : public CADitem
{
public:
    CAD_air_ductFireResistant();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTFIRERESISTANT_H
