#ifndef CAD_ELECTRICAL_CABLETRAY_H
#define CAD_ELECTRICAL_CABLETRAY_H

#include "caditem.h"

class CAD_electrical_cableTray : public CADitem
{
public:
    CAD_electrical_cableTray();
    virtual ~CAD_electrical_cableTray();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ELECTRICAL_CABLETRAY_H
