#ifndef CAD_AIR_THROTTLEVALVE_H
#define CAD_AIR_THROTTLEVALVE_H

#include "caditem.h"

class CAD_air_throttleValve : public CADitem
{
public:
    CAD_air_throttleValve();
    virtual ~CAD_air_throttleValve();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_THROTTLEVALVE_H
