#ifndef CAD_HEATCOOL_CHILLER_H
#define CAD_HEATCOOL_CHILLER_H

#include "caditem.h"

class CAD_heatcool_chiller : public CADitem
{
public:
    CAD_heatcool_chiller();
    virtual ~CAD_heatcool_chiller();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_CHILLER_H
