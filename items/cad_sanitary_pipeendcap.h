#ifndef CAD_SANITARY_PIPEENDCAP_H
#define CAD_SANITARY_PIPEENDCAP_H

#include "caditem.h"

class CAD_sanitary_pipeEndCap : public CADitem
{
public:
    CAD_sanitary_pipeEndCap();
    virtual ~CAD_sanitary_pipeEndCap();
    static QList<CADitem::ItemType> flangable_items();
    static QImage wizardImage();
    virtual void calculate();
    virtual void processWizardInput();
};

#endif // CAD_SANITARY_PIPEENDCAP_H
