#ifndef CAD_AIR_FAN_H
#define CAD_AIR_FAN_H

#include "caditem.h"

class CAD_air_fan : public CADitem
{
public:
    CAD_air_fan();
    virtual ~CAD_air_fan();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_FAN_H
