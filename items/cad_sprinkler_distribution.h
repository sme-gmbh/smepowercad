#ifndef CAD_SPRINKLER_DISTRIBUTION_H
#define CAD_SPRINKLER_DISTRIBUTION_H

#include "caditem.h"

class CAD_sprinkler_distribution : public CADitem
{
public:
    CAD_sprinkler_distribution();
    virtual ~CAD_sprinkler_distribution();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_DISTRIBUTION_H
