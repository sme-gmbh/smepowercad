#ifndef CAD_SPRINKLER_TEECONNECTOR_H
#define CAD_SPRINKLER_TEECONNECTOR_H

#include "caditem.h"

class CAD_sprinkler_teeConnector : public CADitem
{
public:
    CAD_sprinkler_teeConnector();
    virtual ~CAD_sprinkler_teeConnector();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_TEECONNECTOR_H
