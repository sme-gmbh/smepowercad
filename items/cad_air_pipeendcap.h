#ifndef CAD_AIR_PIPEENDCAP_H
#define CAD_AIR_PIPEENDCAP_H

#include "caditem.h"

class CAD_air_pipeEndCap : public CADitem
{
public:
    CAD_air_pipeEndCap();
    virtual ~CAD_air_pipeEndCap();
    virtual QList<CADitem::ItemType> flangable_items();
    virtual QImage wizardImage();
    virtual QString iconPath();
    virtual QString domain();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPEENDCAP_H
