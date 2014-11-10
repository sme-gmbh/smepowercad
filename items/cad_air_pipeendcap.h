#ifndef CAD_AIR_PIPEENDCAP_H
#define CAD_AIR_PIPEENDCAP_H

#include "caditem.h"

class CAD_air_pipeEndCap : public CADitem
{
public:
    CAD_air_pipeEndCap();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_AIR_PIPEENDCAP_H
