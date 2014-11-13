#ifndef CAD_AIR_EQUIPMENTFRAME_H
#define CAD_AIR_EQUIPMENTFRAME_H

#include "caditem.h"

class CAD_air_equipmentFrame : public CADitem
{
public:
    CAD_air_equipmentFrame();
    virtual ~CAD_air_equipmentFrame();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_EQUIPMENTFRAME_H
