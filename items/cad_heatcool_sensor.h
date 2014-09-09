#ifndef CAD_HEATCOOL_SENSOR_H
#define CAD_HEATCOOL_SENSOR_H

#include "caditem.h"

class CAD_heatcool_sensor : public CADitem
{
public:
    CAD_heatcool_sensor();
    virtual void calculate();
};

#endif // CAD_HEATCOOL_SENSOR_H
