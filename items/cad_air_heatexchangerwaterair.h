#ifndef CAD_AIR_HEATEXCHANGERWATERAIR_H
#define CAD_AIR_HEATEXCHANGERWATERAIR_H

#include "caditem.h"

class CAD_air_heatExchangerWaterAir : public CADitem
{
public:
    CAD_air_heatExchangerWaterAir();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_HEATEXCHANGERWATERAIR_H
