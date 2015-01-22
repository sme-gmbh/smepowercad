#ifndef CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H
#define CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H

#include "caditem.h"

class CAD_sprinkler_compressedAirWaterContainer : public CADitem
{
public:
    CAD_sprinkler_compressedAirWaterContainer();
    virtual ~CAD_sprinkler_compressedAirWaterContainer();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SPRINKLER_COMPRESSEDAIRWATERCONTAINER_H
