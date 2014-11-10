#ifndef CAD_AIR_HEATEXCHANGERAIRAIR_H
#define CAD_AIR_HEATEXCHANGERAIRAIR_H

#include "caditem.h"

class CAD_air_heatExchangerAirAir : public CADitem
{
public:
    CAD_air_heatExchangerAirAir();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_HEATEXCHANGERAIRAIR_H
