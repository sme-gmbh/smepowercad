#ifndef CAD_HEATCOOL_STORAGEBOILER_H
#define CAD_HEATCOOL_STORAGEBOILER_H

#include "caditem.h"

class CAD_heatcool_storageBoiler : public CADitem
{
public:
    CAD_heatcool_storageBoiler();
    virtual ~CAD_heatcool_storageBoiler();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_STORAGEBOILER_H
