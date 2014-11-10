#ifndef CAD_SANITARY_ELECTRICWATERHEATER_H
#define CAD_SANITARY_ELECTRICWATERHEATER_H

#include "caditem.h"

class CAD_sanitary_electricWaterHeater : public CADitem
{
public:
    CAD_sanitary_electricWaterHeater();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_ELECTRICWATERHEATER_H
