#ifndef CAD_AIR_DUCTENDPLATE_H
#define CAD_AIR_DUCTENDPLATE_H

#include "caditem.h"

class CAD_air_ductEndPlate : public CADitem
{
public:
    CAD_air_ductEndPlate();
    virtual ~CAD_air_ductEndPlate();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_DUCTENDPLATE_H
