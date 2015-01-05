#ifndef CAD_HEATCOOL_FLANGE_H
#define CAD_HEATCOOL_FLANGE_H

#include "caditem.h"

class CAD_heatcool_flange : public CADitem
{
public:
    CAD_heatcool_flange();
    virtual ~CAD_heatcool_flange();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_FLANGE_H
