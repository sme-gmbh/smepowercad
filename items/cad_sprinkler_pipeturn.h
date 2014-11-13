#ifndef CAD_SPRINKLER_PIPETURN_H
#define CAD_SPRINKLER_PIPETURN_H

#include "caditem.h"

class CAD_sprinkler_pipeTurn : public CADitem
{
public:
    CAD_sprinkler_pipeTurn();
    virtual ~CAD_sprinkler_pipeTurn();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPETURN_H
