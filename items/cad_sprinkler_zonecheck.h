#ifndef CAD_SPRINKLER_ZONECHECK_H
#define CAD_SPRINKLER_ZONECHECK_H

#include "caditem.h"

class CAD_sprinkler_zoneCheck : public CADitem
{
public:
    CAD_sprinkler_zoneCheck();
    virtual ~CAD_sprinkler_zoneCheck();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_ZONECHECK_H
