#ifndef CAD_HEATCOOL_PIPEENDCAP_H
#define CAD_HEATCOOL_PIPEENDCAP_H

#include "caditem.h"

class CAD_heatcool_pipeEndCap : public CADitem
{
public:
    CAD_heatcool_pipeEndCap();
    virtual ~CAD_heatcool_pipeEndCap();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_HEATCOOL_PIPEENDCAP_H
