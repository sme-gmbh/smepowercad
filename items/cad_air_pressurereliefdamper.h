#ifndef CAD_AIR_PRESSURERELIEFDAMPER_H
#define CAD_AIR_PRESSURERELIEFDAMPER_H

#include "caditem.h"

class CAD_air_pressureReliefDamper : public CADitem
{
public:
    CAD_air_pressureReliefDamper();
    virtual ~CAD_air_pressureReliefDamper();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PRESSURERELIEFDAMPER_H
