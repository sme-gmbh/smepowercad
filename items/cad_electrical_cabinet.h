#ifndef CAD_ELECTRICAL_CABINET_H
#define CAD_ELECTRICAL_CABINET_H

#include "caditem.h"

class CAD_electrical_cabinet : public CADitem
{
public:
    CAD_electrical_cabinet();
    virtual ~CAD_electrical_cabinet();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_ELECTRICAL_CABINET_H
