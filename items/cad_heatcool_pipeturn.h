#ifndef CAD_HEATCOOL_PIPETURN_H
#define CAD_HEATCOOL_PIPETURN_H

#include "caditem.h"

class CAD_heatcool_pipeTurn : public CADitem
{
public:
    CAD_heatcool_pipeTurn();
    virtual ~CAD_heatcool_pipeTurn();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPETURN_H
