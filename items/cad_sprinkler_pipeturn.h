#ifndef CAD_SPRINKLER_PIPETURN_H
#define CAD_SPRINKLER_PIPETURN_H

#include "caditem.h"

class CAD_sprinkler_pipeTurn : public CADitem
{
public:
    CAD_sprinkler_pipeTurn();
    virtual ~CAD_sprinkler_pipeTurn();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_PIPETURN_H
