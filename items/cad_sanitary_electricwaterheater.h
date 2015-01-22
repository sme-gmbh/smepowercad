#ifndef CAD_SANITARY_ELECTRICWATERHEATER_H
#define CAD_SANITARY_ELECTRICWATERHEATER_H

#include "caditem.h"

class CAD_sanitary_electricWaterHeater : public CADitem
{
public:
    CAD_sanitary_electricWaterHeater();
    virtual ~CAD_sanitary_electricWaterHeater();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_ELECTRICWATERHEATER_H
