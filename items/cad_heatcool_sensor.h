#ifndef CAD_HEATCOOL_SENSOR_H
#define CAD_HEATCOOL_SENSOR_H

#include "caditem.h"

class CAD_heatcool_sensor : public CADitem
{
public:
    CAD_heatcool_sensor();
    virtual ~CAD_heatcool_sensor();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_SENSOR_H
