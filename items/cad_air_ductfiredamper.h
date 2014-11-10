#ifndef CAD_AIR_DUCTFIREDAMPER_H
#define CAD_AIR_DUCTFIREDAMPER_H

#include "caditem.h"

class CAD_air_ductFireDamper : public CADitem
{
public:
    CAD_air_ductFireDamper();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTFIREDAMPER_H
