#ifndef CAD_HEATCOOL_SENSOR_H
#define CAD_HEATCOOL_SENSOR_H

#include "caditem.h"

class CAD_heatcool_sensor : public CADitem
{
public:
    CAD_heatcool_sensor();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_SENSOR_H
