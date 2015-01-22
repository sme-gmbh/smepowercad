#ifndef CAD_HEATCOOL_FILTER_H
#define CAD_HEATCOOL_FILTER_H

#include "caditem.h"

class CAD_heatcool_filter : public CADitem
{
public:
    CAD_heatcool_filter();
    virtual ~CAD_heatcool_filter();
    virtual QList<CADitemTypes::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual QString description();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_FILTER_H
