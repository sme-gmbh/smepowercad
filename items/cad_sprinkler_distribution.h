#ifndef CAD_SPRINKLER_DISTRIBUTION_H
#define CAD_SPRINKLER_DISTRIBUTION_H

#include "caditem.h"

class CAD_sprinkler_distribution : public CADitem
{
public:
    CAD_sprinkler_distribution();
    virtual ~CAD_sprinkler_distribution();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_DISTRIBUTION_H
