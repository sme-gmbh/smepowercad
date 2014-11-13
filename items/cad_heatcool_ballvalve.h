#ifndef CAD_HEATCOOL_BALLVALVE_H
#define CAD_HEATCOOL_BALLVALVE_H

#include "caditem.h"

class CAD_heatcool_ballValve : public CADitem
{
public:
    CAD_heatcool_ballValve();
    virtual ~CAD_heatcool_ballValve();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_BALLVALVE_H
