#ifndef CAD_AIR_HUMIDIFIER_H
#define CAD_AIR_HUMIDIFIER_H

#include "caditem.h"

class CAD_air_humidifier : public CADitem
{
public:
    CAD_air_humidifier();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_HUMIDIFIER_H
