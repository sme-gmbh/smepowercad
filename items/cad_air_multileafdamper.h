#ifndef CAD_AIR_MULTILEAFDAMPER_H
#define CAD_AIR_MULTILEAFDAMPER_H

#include "caditem.h"

class CAD_air_multiLeafDamper : public CADitem
{
public:
    CAD_air_multiLeafDamper();
    virtual ~CAD_air_multiLeafDamper();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_MULTILEAFDAMPER_H
