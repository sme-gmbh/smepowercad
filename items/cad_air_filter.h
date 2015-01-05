#ifndef CAD_AIR_FILTER_H
#define CAD_AIR_FILTER_H

#include "caditem.h"

class CAD_air_filter : public CADitem
{
public:
    CAD_air_filter();
    virtual ~CAD_air_filter();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_FILTER_H
