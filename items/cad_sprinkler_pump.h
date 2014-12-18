#ifndef CAD_SPRINKLER_PUMP_H
#define CAD_SPRINKLER_PUMP_H

#include "caditem.h"

class CAD_sprinkler_pump : public CADitem
{
public:
    CAD_sprinkler_pump();
    virtual ~CAD_sprinkler_pump();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PUMP_H
